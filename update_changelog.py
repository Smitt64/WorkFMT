#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Универсальный скрипт обновления changelog.
Извлекает описания изменений из макросов (секции $ChangesBegin/$ChangesEnd)
и добавляет их в XML changelog с сохранением XML-entities.

Использование:
    python update_changelog.py --app difftoscript
    python update_changelog.py --app workfmt
"""

import argparse
import os
import re
import sys
import xml.etree.ElementTree as ET
from xml.sax.saxutils import escape


# ============================================================================
# Конфигурация приложений
# ============================================================================
APPS = {
    'difftoscript': {
        'xml_path': r'D:\Work\WorkFMT\DiffToScript\com.rs.fmt.workfmt.difftoscript.xml',
        'macros_dir': r'D:\svn\v6_utils\RsWorkTools\addons\mac\difftoscript',
    },
    'workfmt': {
        'xml_path': r'D:\Work\WorkFMT\FmtLib\com.rs.fmt.workfmt.xml',
        'macros_dir': r'D:\svn\v6_utils\RsWorkTools\addons\mac\fmtcore',
    },
}

# Теги для типов изменений
CHANGE_TYPE_MAP = {
    'ADD': 'add',
    'FIX': 'fix',
    'DEL': 'del',
    'CHANGE': 'change',
}

MACRO_ENCODING = 'cp866'


# ============================================================================
# Работа с XML с сохранением entities
# ============================================================================

def read_xml_preserve_entities(xml_path: str) -> tuple:
    """
    Читает XML, сохраняя информацию о позициях элементов.
    Возвращает (root, tree).
    """
    tree = ET.parse(xml_path)
    root = tree.getroot()
    return root, tree


def write_xml_preserve_entities(root: ET.Element, xml_path: str):
    """
    Записывает XML с сохранением XML-entities (&quot;, &apos;).
    ET автоматически экранирует < > &, но не экранирует " и '.
    Делаем post-processing для сохранения &quot; и &apos;.
    """
    # Записываем во временную строку
    ET.indent(root, space='    ', level=0)
    rough = ET.tostring(root, encoding='unicode', xml_declaration=True)

    # Post-processing: экранируем " и ' в текстовых нодах
    # Стратегия: ищем текст между закрывающим тегом и следующим открывающим/закрывающим тегом
    # и заменяем там " на &quot;, ' на &apos;
    def replace_in_text(match):
        text = match.group(1)
        # Экранируем только кавычки, остальное ET уже сделал
        text = text.replace('"', '&quot;')
        text = text.replace("'", '&apos;')
        return match.group(0)[:match.start(1) - match.start(0)] + text + match.group(0)[match.end(1) - match.start(0):]

    # Заменяем кавычки внутри текстовых нод
    # Шаблон: >текст< или >текст\n<
    result = re.sub(r'>([^<]*?)(?=</)', lambda m: '>' + m.group(1).replace('"', '&quot;').replace("'", '&apos;') + m.group(0)[len(m.group(1))+1:], rough)

    # Убеждаемся, что xml declaration на отдельной строке
    result = result.replace('?><', '?>\n<')

    with open(xml_path, 'w', encoding='utf-8') as f:
        f.write(result)


# ============================================================================
# Парсинг макросов
# ============================================================================

def parse_macro_file(mac_file: str) -> list:
    """
    Парсит один .mac файл.
    Возвращает список словарей:
    [
        {
            'tag': 'add' | 'fix' | ...,
            'description': 'текст описания',
            'table': 'имя_таблицы',
            'macro': 'имя_файла.mac',
        }
    ]
    """
    results = []
    filename = os.path.basename(mac_file)

    with open(mac_file, 'r', encoding=MACRO_ENCODING, errors='replace') as f:
        content = f.read()

    # Ищем $Table
    table_match = re.search(r'^\$Table:\s*(.+)$', content, re.MULTILINE | re.IGNORECASE)
    table_name = table_match.group(1).strip() if table_match else ''

    # Ищем секции $ChangesBegin ... $ChangesEnd
    pattern = re.compile(
        r'^\$ChangesBegin:\s*(.*?)\s*^\$ChangesEnd',
        re.DOTALL | re.MULTILINE | re.IGNORECASE
    )

    for match in pattern.finditer(content):
        section = match.group(1)
        for line in section.splitlines():
            line = line.strip()
            if not line.startswith('-'):
                continue

            line = line[1:].strip()
            if ':' not in line:
                continue

            type_part, desc = line.split(':', 1)
            type_part = type_part.strip().upper()
            desc = desc.strip()

            if not desc:
                continue

            xml_tag = CHANGE_TYPE_MAP.get(type_part, type_part.lower())

            # Формируем описание с информацией о таблице и макросе
            if table_name:
                full_desc = f'Для таблицы {table_name} (макрос {filename}): {desc}'
            else:
                full_desc = f'(макрос {filename}): {desc}'

            results.append({
                'tag': xml_tag,
                'description': full_desc,
                'table': table_name,
                'macro': filename,
            })

    return results


def parse_all_macros(macros_dir: str) -> list:
    """Парсит все .mac файлы в директории."""
    if not os.path.isdir(macros_dir):
        print(f'Директория макросов не найдена: {macros_dir}')
        return []

    changes = []
    mac_files = [
        os.path.join(macros_dir, f)
        for f in os.listdir(macros_dir)
        if f.lower().endswith('.mac')
    ]

    for mac_file in sorted(mac_files):
        file_changes = parse_macro_file(mac_file)
        changes.extend(file_changes)

    return changes


# ============================================================================
# Работа с changelog
# ============================================================================

def get_all_existing_descriptions(root: ET.Element) -> set:
    """Возвращает множество всех существующих описаний (нормализованных)."""
    existing = set()
    for version in root.findall('version'):
        for child in version:
            text = child.text
            if text:
                existing.add(normalize_text(text))
    return existing


def normalize_text(text: str) -> str:
    """Нормализует текст для сравнения."""
    return ' '.join(text.split())


def get_last_version(root: ET.Element) -> ET.Element:
    """Возвращает первую (самую свежую) версию."""
    versions = root.findall('version')
    if not versions:
        raise ValueError('В XML отсутствуют элементы <version>')
    return versions[0]


def add_change_to_version(version_elem: ET.Element, tag: str, text: str):
    """Добавляет запись в указанную версию."""
    new_elem = ET.SubElement(version_elem, tag)
    new_elem.text = text
    # Копируем отступ с предыдущего элемента, если есть
    prev = version_elem[-2] if len(version_elem) >= 2 else None
    if prev is not None:
        new_elem.tail = prev.tail
    else:
        new_elem.tail = '\n    '


# ============================================================================
# Основная логика
# ============================================================================

def process_app(app_name: str):
    """Обрабатывает одно приложение."""
    if app_name not in APPS:
        print(f'Неизвестное приложение: {app_name}')
        print(f'Доступные: {", ".join(APPS.keys())}')
        sys.exit(1)

    config = APPS[app_name]
    xml_path = config['xml_path']
    macros_dir = config['macros_dir']

    print(f'Приложение: {app_name}')
    print(f'XML: {xml_path}')
    print(f'Macros: {macros_dir}')
    print()

    # 1. Парсим макросы
    changes = parse_all_macros(macros_dir)
    if not changes:
        print('Изменения в макросах не найдены.')
        return

    print(f'Найдено записей в макросах: {len(changes)}')
    for ch in changes:
        print(f'  [{ch["tag"]}] {ch["description"][:80]}...')
    print()

    # 2. Загружаем XML
    root, tree = read_xml_preserve_entities(xml_path)

    # 3. Собираем существующие описания
    existing = get_all_existing_descriptions(root)
    print(f'Существующих записей в XML: {len(existing)}')

    # 4. Находим последнюю версию
    last_version = get_last_version(root)
    last_ver_attr = last_version.get('ver', 'unknown')
    print(f'Последняя версия: {last_ver_attr}')
    print()

    # 5. Добавляем новые записи
    added_count = 0
    for ch in changes:
        normalized = normalize_text(ch['description'])
        if normalized in existing:
            print(f'  Пропущено (дубль): [{ch["tag"]}] {ch["description"][:60]}')
            continue

        add_change_to_version(last_version, ch['tag'], ch['description'])
        existing.add(normalized)
        added_count += 1
        print(f'  Добавлено: [{ch["tag"]}] {ch["description"][:80]}')

    # 6. Сохраняем
    if added_count > 0:
        write_xml_preserve_entities(root, xml_path)
        print(f'\nСохранено. Добавлено записей: {added_count}')
    else:
        print('\nНовых записей не найдено. XML не изменён.')


def main():
    parser = argparse.ArgumentParser(
        description='Обновление changelog из макросов'
    )
    parser.add_argument(
        '--app',
        required=True,
        choices=list(APPS.keys()),
        help='Приложение для обновления changelog'
    )
    args = parser.parse_args()

    process_app(args.app)


if __name__ == '__main__':
    main()
