#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Скрипт обновления changelog для DiffToScript.
Извлекает описания изменений из макросов (секции $ChangesBegin/$ChangesEnd)
и добавляет их в com.rs.fmt.workfmt.difftoscript.xml.
"""

import os
import re
import glob
import xml.etree.ElementTree as ET
from xml.dom import minidom


# Пути
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
XML_PATH = os.path.join(SCRIPT_DIR, 'com.rs.fmt.workfmt.difftoscript.xml')
MACROS_DIR = r'D:\svn\v6_utils\RsWorkTools\addons\mac\difftoscript'

# Теги для типов изменений
CHANGE_TYPE_MAP = {
    'ADD': 'add',
    'FIX': 'fix',
    'DEL': 'del',
    'CHANGE': 'change',
}


def parse_changes_from_macros(macros_dir: str) -> list:
    """
    Парсит все .mac файлы в директории и извлекает записи изменений.
    Возвращает список кортежей: (change_type, description)
    """
    changes = []
    mac_files = glob.glob(os.path.join(macros_dir, '*.mac'))

    for mac_file in mac_files:
        with open(mac_file, 'r', encoding='cp866', errors='replace') as f:
            content = f.read()

        # Ищем секции $ChangesBegin ... $ChangesEnd
        pattern = re.compile(
            r'\$ChangesBegin:\s*(.*?)\s*\$ChangesEnd',
            re.DOTALL | re.IGNORECASE
        )

        for match in pattern.finditer(content):
            section = match.group(1)
            # Парсим строки вида: "  - ADD: описание"
            for line in section.splitlines():
                line = line.strip()
                if line.startswith('-'):
                    line = line[1:].strip()
                    # Формат: "TYPE: описание"
                    if ':' in line:
                        type_part, desc = line.split(':', 1)
                        type_part = type_part.strip().upper()
                        desc = desc.strip()
                        if desc:
                            xml_tag = CHANGE_TYPE_MAP.get(type_part, type_part.lower())
                            changes.append((xml_tag, desc))

    return changes


def load_xml(xml_path: str) -> ET.Element:
    """Загружает XML changelog."""
    tree = ET.parse(xml_path)
    return tree


def get_all_existing_descriptions(root: ET.Element) -> set:
    """
    Возвращает множество всех существующих описаний (текст нормализован)
    во всех версиях XML.
    """
    existing = set()
    for version in root.findall('version'):
        for child in version:
            text = child.text
            if text:
                existing.add(normalize_text(text))
    return existing


def normalize_text(text: str) -> str:
    """Нормализует текст для сравнения (убирает лишние пробелы)."""
    return ' '.join(text.split())


def get_last_version(root: ET.Element) -> ET.Element:
    """
    Возвращает первый элемент <version> (самый свежий, т.к. в XML
    версии идут от новых к старым).
    """
    versions = root.findall('version')
    if not versions:
        raise ValueError('В XML отсутствуют элементы <version>')
    return versions[0]


def add_change_to_version(version_elem: ET.Element, tag: str, text: str):
    """Добавляет запись изменения в указанную версию."""
    new_elem = ET.SubElement(version_elem, tag)
    new_elem.text = text
    # Добавляем отступ (форматирование)
    new_elem.tail = '\n    '


def save_xml(tree: ET.ElementTree, xml_path: str):
    """Сохраняет XML с красивым форматированием."""
    root = tree.getroot()
    # Добавляем отступы для новых элементов
    indent_xml(root)
    # Записываем
    rough_string = ET.tostring(root, encoding='utf-8')
    reparsed = minidom.parseString(rough_string)
    pretty = reparsed.toprettyxml(indent='    ', encoding='utf-8')
    # Убираем пустые строки, которые добавляет minidom
    lines = pretty.decode('utf-8').splitlines()
    lines = [line for line in lines if line.strip()]
    with open(xml_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines) + '\n')


def indent_xml(elem, level=0):
    """Добавляет отступы в XML для красивого вывода."""
    i = '\n' + level * '    '
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + '    '
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for child in elem:
            indent_xml(child, level + 1)
        if not child.tail or not child.tail.strip():
            child.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i


def main():
    print(f'XML: {XML_PATH}')
    print(f'Macros dir: {MACROS_DIR}')

    # 1. Парсим изменения из макросов
    changes = parse_changes_from_macros(MACROS_DIR)
    if not changes:
        print('Изменения в макросах не найдены.')
        return

    print(f'Найдено записей в макросах: {len(changes)}')
    for tag, desc in changes:
        print(f'  [{tag}] {desc}')

    # 2. Загружаем XML
    tree = load_xml(XML_PATH)
    root = tree.getroot()

    # 3. Получаем все существующие описания (для проверки дублей)
    existing = get_all_existing_descriptions(root)
    print(f'Существующих записей в XML: {len(existing)}')

    # 4. Находим последнюю версию
    last_version = get_last_version(root)
    last_ver_attr = last_version.get('ver', 'unknown')
    print(f'Последняя версия: {last_ver_attr}')

    # 5. Добавляем новые записи (без дублей)
    added_count = 0
    for tag, desc in changes:
        normalized = normalize_text(desc)
        if normalized in existing:
            print(f'  Пропущено (дубль): [{tag}] {desc[:60]}...')
            continue

        add_change_to_version(last_version, tag, desc)
        existing.add(normalized)
        added_count += 1
        print(f'  Добавлено: [{tag}] {desc[:60]}...')

    # 6. Сохраняем XML
    if added_count > 0:
        save_xml(tree, XML_PATH)
        print(f'\nСохранено. Добавлено записей: {added_count}')
    else:
        print('\nНовых записей не найдено. XML не изменён.')


if __name__ == '__main__':
    main()
