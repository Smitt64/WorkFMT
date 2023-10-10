# https://habr.com/ru/companies/vdsina/articles/557316/
import sys
import os
import enchant
import csv
import argparse

# Создаем объект словаря
d = enchant.Dict("en_US")

def capitalize_words(s):
    # разбиваем строку на список слов
    words = s.split()
    # создаем новый список для измененных слов
    capitalized_words = []
    # проходим по каждому слову в списке
    for word in words:
        # делаем первую букву заглавной, а остальные - прописными
        capitalized_word = word.capitalize()
        # добавляем измененное слово в новый список
        capitalized_words.append(capitalized_word)
    # объединяем список слов в одну строку с помощью пробелов
    capitalized_string = ''.join(capitalized_words)
    # возвращаем измененную строку
    return capitalized_string

def LongestString(strings):
    # Инициализируем переменную для хранения самой длинной строки
    longest = ""
    
    # Проходим по каждой строке в массиве
    for string in strings:
        # Если текущая строка длиннее, чем предыдущая самая длинная, обновляем значение переменной longest
        if len(string) > len(longest):
            longest = string
    
    # Возвращаем самую длинную строку
    return longest

def is_same_char(string):
    if len(set(string)) == 1:
        return True
    else:
        return False

def FindNextMatch(current_word, chunk):
    words = []
    word = current_word

    for index, char in enumerate(chunk):
        word += char

        if d.check(word):
            words.append(word)
    
    return LongestString(words)

def split_words(string):
    # Инициализируем переменные
    words = []
    current_word = ""
    processed = ''
    
    # Проходим по каждому символу в строке
    pos = 0
    while (pos < len(string)):
        word = ''
        current_word += string[pos:pos + 1]

        if not current_word.isnumeric():
            word = FindNextMatch(current_word, string[pos + 1:])

            if word[-1:] == 's':
                word2 = FindNextMatch('s', string[pos + len(word):])

                if word2:
                    processed += word[0:len(word)-1].capitalize()
                    processed += word2.capitalize()

                    pos += len(word) + len(word2) - 2
                    word = None

        if not str(word).isalpha():
            processed += current_word

        if not word:
            pos += 1
            current_word = ''
        else:
            pos += len(word)
            current_word = ''
            processed += word.capitalize()
            words.append(word)
    
    # Возвращаем список слов
    return processed

# Пример использования
d.add_to_session('ret')
d.add_to_session('calc')
d.add_to_session('oper')
d.add_to_session('status')
d.add_to_session('unknow')
d.add_to_session('util')
d.add_to_session('secur')
d.add_to_session('sum')
d.add_to_session('guid')
d.add_to_session('fiid')
d.add_to_session('rouble')
d.add_to_session('attr')
d.add_to_session('ofshore')
d.add_to_session('param')
d.add_to_session('taxinstitution')

if len(sys.argv) > 1:
    string = sys.argv[1]

    parser = argparse.ArgumentParser(description='Optional app description')
    parser.add_argument('--field', type=str, nargs='?', help='A required string positional argument')
    parser.add_argument('--fieldlist', type=str, nargs='?', help='A required string positional argument')

    args = parser.parse_args()

    if (not args.field) and (not args.fieldlist):
        print('Error: Input string not passed')
        exit(1)
    
    if args.field is not None:
        result = split_words(args.field)
        print(result)

    if args.fieldlist is not None:
        results = []
        fields = args.fieldlist.split(';')

        for field in fields:
            word = split_words(field)
            results.append(word)

        print(';'.join(results))

else:
    string = 'taxinstitution'
    result = split_words(string)
    print(result)

    '''file1 = open("D:/Work/WorkFMT/bin/1fmt_fields.txt", "r")

    fieldnames = ['source', 'destination']
    
    with open('D:/Work/WorkFMT/bin/1fmt_fields.csv', 'w', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        lines = file1.readlines()
        for line in lines:
            splitted = split_words(line.strip())

            writer.writerow({'source': line.strip(), 'destination': splitted})'''
#debitcurrcode