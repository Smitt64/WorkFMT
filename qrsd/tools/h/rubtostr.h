/*******************************************************************************
 FILE         :   RUBTOSTR.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Инструментарий для работы с суммами

 PROGRAMMED BY:   VECsoft

 CREATION DATE:   14.10.1992

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rubtostr.h $
                  $Revision: 16 $
                  $Date: 12.05.00 13:29 $
                  $Author: Lebedev $
                  $Modtime: 11.05.00 12:07 $

-----------------------------------------------------------------------
 September 20, 1996 Николай Митрофанов, R-Style Software Lab
 - добавил InitLocaleData - чтение параметров национального языка
 - изменил rubtostr       - теперь на национальном языке
                          - теперь до 999 триллионов
 May, 2000
   Все функции получают значения lmoney без предварительного деления на 100.
*******************************************************************************/

#ifndef __RUBTOSTR_H
#define __RUBTOSTR_H

#include "tooldef.h"
#include "rstypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TNameData
      {
       char  Name1[80];         // доллар
       char  Name2[80];         // доллара
       char  Name0[80];         // долларов
       int   Rod;               // род : 1-муж. 2-жен.
      } TNameData;

typedef struct TCurData
      {
       void      *NextCur;      // указатель на следующий элемент списка
                                // NULL - конец списка
       int        CurCode;      // код валюты
       TNameData  NameA;        // таблица падежных форм названия единицы валюты
       TNameData  NameB;        // -.- под'единицы валюты (цент)
      } TCurData;

typedef struct TLocaleData
      {
       char       Language[20];      // язык
       char       Algoritm[20];      // алгоритм
       TCurData   CurList;           // список валют

       // далее все написания слов

       char       Neg[20];           // минус
       char       Rubx[20];          // руб.
       char       Kopx[20];          // коп.

       TNameData  Rub;               // рубль
       TNameData  Kop;               // копейка
       TNameData  Tsc;               // тысяча
       TNameData  Mln;               // миллион
       TNameData  Mrd;               // миллиард
       TNameData  Trn;               // триллион

       char       M1[13][20];        // 12 месяцев
       char       D1[32][20];        // 31 день
       char       Year[20];          // год
       char       N1a[20];           // одна
       char       N1o[20];           // одно
       char       N2a[20];           // две
       char       N1[20][20];        // один..девятнадцать
       char       N10[10][20];       // числа 20..90
       char       N100[10][20];      // числа 100..900

       TNameData  Ften;             // десятая
       TNameData  Fhnd;             // сотая
       TNameData  Ftsc;             // тысячная
       TNameData  Ftts;             // десятитысячная
       TNameData  Fhts;             // стотысячная
       TNameData  Fmln;             // миллионная
       TNameData  Ftml;             // десятимиллионная
       TNameData  Fhml;             // стомиллионная
       TNameData  Fmrd;             // миллиардная
       TNameData  Ftmr;             // десятимиллиардная
       TNameData  Fhmr;             // стомиллиардная
       TNameData  Ftrn;             // триллионная
       TNameData  Fttr;             // десятитриллионная
       TNameData  Fhtr;             // стотриллионная

       TNameData  Ival;             // целая
      } TLocaleData;

// ==========================================================

_TOOLEXP void InitLocaleData(void); // Читаем LOCALE.INI
_TOOLEXP void InitLocaleDataEx(const char *cnfFile);
_TOOLEXP void DoneLocaleData(void ); // Освобождаем ресурсы
_TOOLEXP TLocaleData *GetLocaleData (void);
_TOOLEXP void ReadLocaleData(const char *cnfFile, TLocaleData *data);
_TOOLEXP void CloseLocaleData(TLocaleData *data);

// ==========================================================

// Преобразование суммы цифрами в сумму прописью
// Максимальная сумма 999,999,999,999,999.99
// total: сумма в рублях (копейки в дробной части)
// str:   cумма прописью (макс.длина 200 символов)
// если имеется файл LOCALE.INI то из него берутся написания всех слов
// т.е. для Украины получится например "Двадцять двi гривни 41 копийок"

#if !defined(NUMERIC_AS_MONEY)
    // output: str="Двадцать два рубля 41 копейка"
    _TOOLEXP void LocalRubToStr(lmoney total, char *str);

    // output: str="Двадцать два доллара 41 цент"
    _TOOLEXP void LocalCurToStr(lmoney  total,    // сумма в единицах валюты
                                char   *str,      // cумма прописью
                                int     CurCode   // код валюты (0-национальная валюта)
                               );

    // output: str="Двадцать два руб. 41 коп."
    _TOOLEXP void rubtostr(lmoney total, char *str);

    // output: str="Двадцать два .41 USD"
    _TOOLEXP void curtostr(lmoney  total,    // сумма в рублях
                           char   *str,      // cумма прописью
                           char   *CurNam    // название валюты e.g. "USD"
                         );

    // output: str="Двадцать два", kop="41"
    _TOOLEXP void SumToStr(lmoney total, char *str, char *kop);

    _TOOLEXP void CurSumToStr(lmoney total, char *str, char *kop, int code);

    // Количество некоторых предметов прописью
    // output: str="Двадцать два болта"
    _TOOLEXP  void NumToStr(lmoney  total,
                            char   *str,
                            char   *name1,         // input: "болт"
                            char   *name2,         // input: "болта"
                            char   *name0,         // input: "болтов"
                            int     Rod);          // input: 1=мужской род, 2=женский род
#endif //!defined(NUMERIC_AS_MONEY)

// Количество некоторых предметов прописью(но с учетом чисел с плавающей точкой)
// output: str="Двадцать две целых три сотых болта"
_TOOLEXP  void NumToStrEx(double total,
                          char *str,
                          char *name1,         // input: "болт"
                          char *name2,         // input: "болта"
                          char *name0,         // input: "болтов"
                          int Rod,             // input: 1=мужской род, 2=женский род
                          int Prec);           // число знаков после запятой

// Добавляет пробел и слово wrd к строке str
_TOOLEXP  void AddWord(char *str, char *wrd);

// Поднять первую букву в сроке str
_TOOLEXP  void Up1Letter(char *str);


_TOOLEXP char *MonthName(int   n,     // месяц
                         char *mon);  // в текстовом виде

_TOOLEXP char *DayName(int   n,       // день
                       char *day);    // в текстовом виде

_TOOLEXP char *YearName(char  *year); // в текстовом виде

#ifdef __cplusplus
}
#endif

// ==========================================================

#if defined(USE_NUMERIC)

    #ifdef __cplusplus

        _TOOLEXP void LocalRubToStr   // output: str="Двадцать два рубля 41 копейка"
        (
           const Numeric& total,
           char* str
        );

        _TOOLEXP void LocalRubToStrEx
        (
           const Numeric& total,
           char* str,
           int   prec                 // число знаков для копеек
        );

        _TOOLEXP void LocalCurToStr   // output: str="Двадцать два доллара 41 цент"
        (
           const Numeric& total,      // сумма в единицах валюты
           char* str,                 // cумма прописью
           int   CurCode              // код валюты (0-национальная валюта)
        );

        _TOOLEXP void LocalCurToStrEx
        (
           const Numeric& total,      // сумма в единицах валюты
           char* str,                 // cумма прописью
           int   curCode,             // код валюты (0-национальная валюта)
           int   prec                 // число знаков для копеек
        );

        _TOOLEXP void rubtostr        // output: str="Двадцать два руб. 41 коп."
        (
          const Numeric &total,
          char* str
        );

        _TOOLEXP void rubtostrEx      // output: str="Двадцать два руб. 41 коп."
        (
           const Numeric &total,
           char* str,
           int   prec                 // число знаков для копеек
        );

        _TOOLEXP void curtostr        // output: str="Двадцать два .41 USD"
        (
           const Numeric &total,      // сумма в рублях
           char* str,                 // cумма прописью
           char* CurNam               // название валюты e.g. "USD"
        );

        _TOOLEXP void SumToStr        // output: str="Двадцать два", kop="41"
        (
           const Numeric &total,
           char* str,
           char* kop
        );

        _TOOLEXP void CurSumToStr
        (
           const Numeric& total,
           char* str,
           char* kop,
           int   code
        );

        _TOOLEXP void CurSumToStrEx
              (
              const Numeric& total,
              char* str,
              char* kop,
              int   prec,
              int   code
              );

        _TOOLEXP void SumToStrEx      // output: str="Двадцать два", kop="41"
        (
           const Numeric &total,
           char* str,
           char* kop,
           int   prec
        );

        // Количество некоторых предметов прописью
        _TOOLEXP void NumToStr        // output: str="Двадцать два болта"
        (
          const Numeric &total,
          char *str,
          char *name1,                // input: "болт"
          char *name2,                // input: "болта"
          char *name0,                // input: "болтов"
          int Rod                     // input: 1=мужской род, 2=женский род
        );

        // Количество некоторых предметов прописью(но с учетом чисел с плавающей точкой)
        _TOOLEXP  void NumToStrEx     // output: str="Двадцать две целых три сотых болта"
        ( 
          const Numeric &total,
          char *str,
          char *name1,                // input: "болт"
          char *name2,                // input: "болта"
          char *name0,                // input: "болтов"
          int Rod,                    // input: 1=мужской род, 2=женский род
          int Prec                    // число знаков после запятой
        );

    #endif // __cplusplus

#endif // defined(USE_NUMERIC)

// ==========================================================

#endif // __RUBTOSTR_H

/* <-- EOF --> */