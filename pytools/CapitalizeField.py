# https://habr.com/ru/companies/vdsina/articles/557316/

"""
Утилита для разбиения составных имен полей банковской системы на отдельные слова.
Поддерживает различные форматы: CamelCase, snake_case, с префиксом t_ и без.
"""

import sys
import os
import argparse
import re
import json
import select
import time
import traceback
import signal
from typing import List, Tuple, Set
from dataclasses import dataclass, asdict
import enchant

@dataclass
class SplitResult:
    """Результат разбиения"""
    original: str
    words: List[str]
    score: float
    method: str
    has_prefix: bool = False
    confidence: float = 1.0
    
    def get_readable_form(self) -> str:
        return ' '.join([w for w in self.words if w != '_'])
    
    def to_dict(self) -> dict:
        return {
            'success': True,
            'original': self.original,
            'formatted': self.get_formatted(),
            'words': self.words,
            'readable': self.get_readable_form(),
            'confidence': self.confidence,
            'method': self.method
        }
    
    def get_formatted(self) -> str:
        formatted = ''.join(self.words)
        if self.has_prefix:
            if self.original.startswith('T_'):
                return f"T_{formatted}"
            return f"t_{formatted}"
        return formatted


class BankFieldSplitter:
    """Специализированный сплиттер для банковских полей"""
    
    BUILTIN_TERMS: Set[str] = {
        'ret', 'calc', 'oper', 'status', 'unknow', 'util', 'secur', 
        'sum', 'guid', 'fiid', 'rouble', 'attr', 'ofshore', 'param', 
        'taxinstitution',
        'acc', 'acct', 'acctrn', 'accsub', 'accvanl',
        'sbp', 'qr', 'knf', 'rls', 'prz', 'bwp', 'rvps',
        'fssp', 'escr', 'okato', 'oktmo', 'kladr', 'fias',
        'inn', 'kpp', 'ogrn', 'snils', 'bic', 'swift', 'iban',
        'payer', 'receiver', 'beneficiary', 'recipient',
        'debit', 'credit', 'amount', 'balance', 'limit', 'rate',
        'doc', 'date', 'time', 'period', 'number', 'code', 'type',
        'kind', 'ref', 'num', 'flag', 'mode',
        'card', 'embossing', 'cash', 'symbol',
        'auth', 'confirm', 'approve', 'process',
        'tax', 'nal', 'budget', 'payment', 'pay',
        'name', 'fio', 'surname', 'patronymic', 'address',
        'bank', 'account', 'department', 'branch',
        'dmInActive', 'lmInActive', 'dmDebet', 'dmCredit',
        'subdprt', 'repper', 'condlim', 'dmFinalCredit',
    }
    
    UPPERCASE_ABBR: Set[str] = {
        'id', 'uuid', 'guid', 'inn', 'kpp', 'ogrn', 'snils', 'bic', 'swift',
        'iban', 'fiid', 'sbp', 'qr', 'abs', 'cbu', 'cbr', 'fssp', 'fns', 'fsfm',
        'escr', 'rls', 'xml', 'xls', 'csv', 'pdf', 'html', 'ip', 'mac', 'pc',
        'os', 'db', 'ui', 'api', 'sdk', 'url', 'ftp', 'http', 'https', 'ssl',
        'tls', 'ssh', 'tel', 'fax', 'sms', 'mms', 'atm', 'pos', 'pin', 'cvv',
        'cvc', 'pan', 'rn', 'rs', 'ts', 'vs', 'nds', 'np', 'fl', 'ul',
        'prz', 'knf', 'rls', 'kkm', 'kkt', 'gb', 'mh', 'spr', 'spv', 'spo',
        'spd', 'acs', 'bwp', 'rvps', 'dm', 'lm', 'fi'
    }
    
    CAPITALIZE_WORDS: Set[str] = {
        'rec', 'acc', 'fnd', 'ctg', 'payer', 'receiver', 'bank', 'account',
        'client', 'payment', 'date', 'time', 'amount', 'sum', 'code', 'type',
        'kind', 'number', 'name', 'address', 'department', 'branch',
        'ret', 'calc', 'oper', 'status', 'secur', 'util', 'guid', 'attr',
        'param', 'taxinstitution', 'month', 'year', 'day', 'period',
        'begin', 'end', 'start', 'finish', 'first', 'last', 'old', 'new',
        'main', 'base', 'primary', 'secondary', 'additional', 'extra'
    }
    
    def __init__(self, dict_name: str = "en_US"):
        self.dictionary = enchant.Dict(dict_name)
        self.cache = {}
        self.min_word_length = 2
        self.max_word_length = 20
        
        for term in self.BUILTIN_TERMS:
            self.dictionary.add_to_session(term)
        for term in self.CAPITALIZE_WORDS:
            self.dictionary.add_to_session(term)
        for term in self.UPPERCASE_ABBR:
            self.dictionary.add_to_session(term)
    
    def preprocess(self, field_name: str) -> Tuple[str, bool]:
        has_prefix = False
        
        if field_name.startswith('t_'):
            has_prefix = True
            field_name = field_name[2:]
        elif field_name.startswith('T_'):
            has_prefix = True
            field_name = field_name[2:]
        
        return field_name, has_prefix
    
    def split_snake_case(self, text: str) -> List[str]:
        parts = re.split(r'(_+)', text)
        result = []
        for part in parts:
            if part:
                result.append(part)
        return result
    
    def split_camel_case(self, text: str) -> List[str]:
        if not text:
            return []
        
        text_with_boundaries = re.sub(
            r'(?<=[a-z])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])|(?<=[0-9])(?=[A-Za-z])|(?<=[A-Za-z])(?=[0-9])', 
            '|', text
        )
        words = text_with_boundaries.split('|')
        
        result = []
        for word in words:
            if word.isupper() and len(word) > 4:
                subwords = self.find_words_in_string(word.lower())
                result.extend(subwords)
            else:
                result.append(word)
        
        return result
    
    def find_words_in_string(self, text: str) -> List[str]:
        if not text:
            return []
        
        n = len(text)
        dp = [(0, -1) for _ in range(n + 1)]
        
        for i in range(n):
            if dp[i][0] is None:
                continue
            
            for j in range(i + 1, min(n, i + self.max_word_length) + 1):
                word = text[i:j]
                
                if (word in self.BUILTIN_TERMS or 
                    word in self.UPPERCASE_ABBR or 
                    word in self.CAPITALIZE_WORDS or
                    self.dictionary.check(word) or
                    word.isdigit()):
                    
                    score = dp[i][0] + len(word) ** 2
                    if score > dp[j][0]:
                        dp[j] = (score, i)
        
        result = []
        pos = n
        while pos > 0:
            prev = dp[pos][1]
            if prev == -1:
                result = list(text)
                break
            word = text[prev:pos]
            result.append(word)
            pos = prev
        
        return list(reversed(result))
    
    def format_word(self, word: str) -> str:
        if not word:
            return word
        
        word_lower = word.lower()
        
        if word in ('_', '__', '___'):
            return word
        
        if word.isdigit():
            return word
        
        if word_lower in self.UPPERCASE_ABBR:
            return word_lower.upper()
        
        if word_lower in self.CAPITALIZE_WORDS:
            return word_lower.capitalize()
        
        if word[0].isupper() and any(c.islower() for c in word[1:]):
            return word
        
        return word_lower.capitalize()
    
    def calculate_confidence_score(self, result: SplitResult) -> float:
        if not result.words:
            return 0.0
        
        words = [w for w in result.words if w != '_']
        if not words:
            return 0.0
        
        valid_words = 0
        for word in words:
            word_lower = word.lower()
            if (word_lower in self.BUILTIN_TERMS or
                word_lower in self.UPPERCASE_ABBR or
                word_lower in self.CAPITALIZE_WORDS or
                word.isdigit() or
                self.dictionary.check(word_lower)):
                valid_words += 1
        
        return valid_words / len(words)
    
    def split_field(self, field_name: str) -> SplitResult:
        self.cache = {}
        
        original = field_name
        name_without_prefix, has_prefix = self.preprocess(field_name)
        
        if '_' in name_without_prefix:
            parts = self.split_snake_case(name_without_prefix)
            result_words = []
            
            for part in parts:
                if part == '_':
                    result_words.append('_')
                else:
                    result_words.append(self.format_word(part))
            method = 'snake'
        
        elif any(c.isupper() for c in name_without_prefix):
            words = self.split_camel_case(name_without_prefix)
            result_words = []
            for word in words:
                if word.isupper() and len(word) > 4:
                    subwords = self.find_words_in_string(word.lower())
                    for subword in subwords:
                        result_words.append(self.format_word(subword))
                else:
                    result_words.append(self.format_word(word))
            method = 'camel'
        
        else:
            words = self.find_words_in_string(name_without_prefix)
            result_words = [self.format_word(w) for w in words]
            method = 'dp'
        
        result_words = [w for w in result_words if w]
        score = sum(len(w) for w in result_words if w.isalpha())
        
        result = SplitResult(
            original=original,
            words=result_words,
            score=score,
            method=method,
            has_prefix=has_prefix
        )
        
        result.confidence = self.calculate_confidence_score(result)
        
        return result
    
    def format_result(self, result: SplitResult) -> str:
        formatted = ''.join(result.words)
        if result.has_prefix:
            if result.original.startswith('T_'):
                return f"T_{formatted}"
            return f"t_{formatted}"
        return formatted


# Флаг для контроля работы основного цикла
running = True

def signal_handler(sig, frame):
    """Обработчик сигналов для graceful shutdown"""
    global running
    sys.stderr.write("\nReceived interrupt signal, shutting down...\n")
    sys.stderr.flush()
    running = False

def json_mode_simple():
    """
    Простой режим с блокирующим чтением.
    Идеально подходит для работы через pipe (echo | program)
    """
    # Отключаем буферизацию
    if hasattr(sys.stdout, 'reconfigure'):
        sys.stdout.reconfigure(line_buffering=True)
        sys.stderr.reconfigure(line_buffering=True)
    
    sys.stderr.write(f"JSON mode started. PID: {os.getpid()}\n")
    sys.stderr.flush()
    
    try:
        splitter = BankFieldSplitter()
        sys.stderr.write("Splitter initialized successfully\n")
        sys.stderr.flush()
    except Exception as e:
        sys.stderr.write(f"Failed to initialize splitter: {e}\n")
        sys.stderr.flush()
        return
    
    # Просто читаем строки пока есть ввод
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
            
        # Проверяем команду выхода
        if line == '{"command":"exit"}' or line == 'exit':
            sys.stderr.write("Exit command received, exiting...\n")
            sys.stderr.flush()
            break
            
        # Обрабатываем запрос
        process_line(line, splitter)
    
    sys.stderr.write("JSON mode finished\n")
    sys.stderr.flush()


def json_mode():
    """Основная функция"""
    if sys.platform == "win32":
        json_mode_simple()  # Для Windows используем простой вариант
    else:
        # Для Unix можно оставить сложную логику или тоже использовать простой вариант
        json_mode_simple()

def process_line(line, splitter):
    """Обрабатывает одну строку запроса"""
    try:
        request = json.loads(line)
        request_id = request.get('requestId', -1)
        
        if 'field' not in request:
            response = {
                'success': False,
                'error': 'Missing "field" parameter',
                'requestId': request_id
            }
        else:
            field_name = request['field']
            
            if request.get('batch', False):
                if not isinstance(field_name, list):
                    response = {
                        'success': False,
                        'error': 'For batch mode, "field" must be a list',
                        'requestId': request_id
                    }
                else:
                    results = []
                    for f in field_name:
                        try:
                            result = splitter.split_field(f)
                            results.append(result.to_dict())
                        except Exception as e:
                            results.append({
                                'success': False,
                                'error': str(e),
                                'original': f
                            })
                    
                    response = {
                        'success': True,
                        'batch': True,
                        'results': results,
                        'requestId': request_id
                    }
            else:
                try:
                    result = splitter.split_field(field_name)
                    response = result.to_dict()
                    response['requestId'] = request_id
                except Exception as e:
                    response = {
                        'success': False,
                        'error': str(e),
                        'original': field_name,
                        'requestId': request_id
                    }
        
        # Отправляем ответ
        response_json = json.dumps(response, ensure_ascii=False)
        sys.stdout.write(response_json + '\n')
        sys.stdout.flush()
        
        sys.stderr.write(f"Processed request {request_id}\n")
        sys.stderr.flush()
        
    except json.JSONDecodeError as e:
        error_response = {
            'success': False,
            'error': f'Invalid JSON: {str(e)}',
            'original': line[:100],
            'requestId': -1
        }
        sys.stdout.write(json.dumps(error_response) + '\n')
        sys.stdout.flush()
        
        sys.stderr.write(f"JSON parse error: {e}\n")
        sys.stderr.flush()
        
    except Exception as e:
        error_response = {
            'success': False,
            'error': f'Processing error: {str(e)}',
            'requestId': -1
        }
        sys.stdout.write(json.dumps(error_response) + '\n')
        sys.stdout.flush()
        
        sys.stderr.write(f"Processing error: {e}\n")
        traceback.print_exc(file=sys.stderr)
        sys.stderr.flush()


def main():
    parser = argparse.ArgumentParser(
        description='Разбиение составных имен полей'
    )
    
    group = parser.add_mutually_exclusive_group()
    group.add_argument('--field', type=str, help='Одно поле для обработки')
    group.add_argument('--fieldlist', type=str, help='Список полей через разделитель')
    group.add_argument('--test', action='store_true', help='Запустить тестовые примеры')
    group.add_argument('--json', action='store_true', help='JSON режим (чтение из stdin, запись в stdout)')
    
    parser.add_argument('--separator', type=str, default=';',
                       help='Разделитель для fieldlist (по умолчанию ";")')
    parser.add_argument('--dict', type=str, default='en_US',
                       help='Словарь для проверки слов (по умолчанию en_US)')
    parser.add_argument('--detailed', action='store_true',
                       help='Детальный вывод')
    parser.add_argument('--show-confidence', action='store_true',
                       help='Показать уверенность в разбиении')
    
    args = parser.parse_args()
    
    if args.json:
        json_mode()
        return
    
    splitter = BankFieldSplitter(args.dict)
    
    if args.test:
        test_cases = [
            "t_receiveraccount",
            "payerbankid",
            "t_rec_acc_fnd_ctg",
            "t_Rec_Acc_Fnd_Ctg",
            "PayerBankID",
            "T_clientid",
            "t_Rec_Acc_Fnd_Ctg_123",
            "dmInActive",
            "lmInActive",
            "dmDebet",
            "dmCredit",
            "subdprt",
            "repper",
            "condlim",
            "dmFinalCredit"
        ]
        
        print("=" * 90)
        print("ТЕСТОВЫЕ ПРИМЕРЫ")
        print("=" * 90)
        
        for field in test_cases:
            result = splitter.split_field(field)
            formatted = splitter.format_result(result)
            readable = result.get_readable_form()
            
            if args.detailed or args.show_confidence:
                confidence_str = f" (уверенность: {result.confidence:.2%})" if args.show_confidence else ""
                method_str = f" [метод: {result.method}]"
                print(f"{field:<30} -> {formatted:<40} | [{readable}]{confidence_str}{method_str}")
            else:
                print(f"{field} -> {formatted}")
        
        print("=" * 90)
        return
    
    if not args.field and not args.fieldlist:
        test_string = 'taxinstitution'
        result = splitter.split_field(test_string)
        print(splitter.format_result(result))
        return
    
    if args.field:
        result = splitter.split_field(args.field)
        if args.detailed:
            words_str = ' '.join([w for w in result.words if w != '_'])
            confidence_str = f" (уверенность: {result.confidence:.2%})" if args.show_confidence else ""
            print(f"{result.original} -> {splitter.format_result(result)} | [{words_str}]{confidence_str}")
        else:
            print(splitter.format_result(result))
    
    if args.fieldlist:
        fields = args.fieldlist.split(args.separator)
        results = []
        for field in fields:
            field = field.strip()
            if field:
                result = splitter.split_field(field)
                if args.detailed:
                    words_str = ' '.join([w for w in result.words if w != '_'])
                    confidence_str = f" (уверенность: {result.confidence:.2%})" if args.show_confidence else ""
                    results.append(f"{result.original} -> {splitter.format_result(result)} | [{words_str}]{confidence_str}")
                else:
                    results.append(splitter.format_result(result))
        
        print(args.separator.join(results))


if __name__ == "__main__":
    main()