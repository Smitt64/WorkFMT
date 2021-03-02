/*******************************************************************************
 FILE         :   RUBTOSTR.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   �����㬥��਩ ��� ࠡ��� � �㬬���

 PROGRAMMED BY:   VECsoft

 CREATION DATE:   14.10.1992

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rubtostr.h $
                  $Revision: 16 $
                  $Date: 12.05.00 13:29 $
                  $Author: Lebedev $
                  $Modtime: 11.05.00 12:07 $

-----------------------------------------------------------------------
 September 20, 1996 ������� ����䠭��, R-Style Software Lab
 - ������� InitLocaleData - �⥭�� ��ࠬ��஢ ��樮���쭮�� �몠
 - ������� rubtostr       - ⥯��� �� ��樮���쭮� �몥
                          - ⥯��� �� 999 �ਫ������
 May, 2000
   �� �㭪樨 ������� ���祭�� lmoney ��� �।���⥫쭮�� ������� �� 100.
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
       char  Name1[80];         // ������
       char  Name2[80];         // ������
       char  Name0[80];         // �����஢
       int   Rod;               // த : 1-��. 2-���.
      } TNameData;

typedef struct TCurData
      {
       void      *NextCur;      // 㪠��⥫� �� ᫥���騩 ����� ᯨ᪠
                                // NULL - ����� ᯨ᪠
       int        CurCode;      // ��� ������
       TNameData  NameA;        // ⠡��� �������� �� �������� ������� ������
       TNameData  NameB;        // -.- ���'������� ������ (業�)
      } TCurData;

typedef struct TLocaleData
      {
       char       Language[20];      // ��
       char       Algoritm[20];      // ������
       TCurData   CurList;           // ᯨ᮪ �����

       // ����� �� ����ᠭ�� ᫮�

       char       Neg[20];           // �����
       char       Rubx[20];          // ��.
       char       Kopx[20];          // ���.

       TNameData  Rub;               // �㡫�
       TNameData  Kop;               // �������
       TNameData  Tsc;               // �����
       TNameData  Mln;               // �������
       TNameData  Mrd;               // �������
       TNameData  Trn;               // �ਫ����

       char       M1[13][20];        // 12 ����楢
       char       D1[32][20];        // 31 ����
       char       Year[20];          // ���
       char       N1a[20];           // ����
       char       N1o[20];           // ����
       char       N2a[20];           // ���
       char       N1[20][20];        // ����..����⭠����
       char       N10[10][20];       // �᫠ 20..90
       char       N100[10][20];      // �᫠ 100..900

       TNameData  Ften;             // ������
       TNameData  Fhnd;             // ���
       TNameData  Ftsc;             // ����筠�
       TNameData  Ftts;             // ���������筠�
       TNameData  Fhts;             // ������筠�
       TNameData  Fmln;             // ����������
       TNameData  Ftml;             // ����⨬���������
       TNameData  Fhml;             // �⮬���������
       TNameData  Fmrd;             // ������भ��
       TNameData  Ftmr;             // ����⨬�����भ��
       TNameData  Fhmr;             // �⮬�����भ��
       TNameData  Ftrn;             // �ਫ�������
       TNameData  Fttr;             // ������ਫ�������
       TNameData  Fhtr;             // ���ਫ�������

       TNameData  Ival;             // 楫��
      } TLocaleData;

// ==========================================================

_TOOLEXP void InitLocaleData(void); // ��⠥� LOCALE.INI
_TOOLEXP void InitLocaleDataEx(const char *cnfFile);
_TOOLEXP void DoneLocaleData(void ); // �᢮������� ������
_TOOLEXP TLocaleData *GetLocaleData (void);
_TOOLEXP void ReadLocaleData(const char *cnfFile, TLocaleData *data);
_TOOLEXP void CloseLocaleData(TLocaleData *data);

// ==========================================================

// �८�ࠧ������ �㬬� ��ࠬ� � �㬬� �ய����
// ���ᨬ��쭠� �㬬� 999,999,999,999,999.99
// total: �㬬� � �㡫�� (������� � �஡��� ���)
// str:   c㬬� �ய���� (����.����� 200 ᨬ�����)
// �᫨ ������� 䠩� LOCALE.INI � �� ���� ������� ����ᠭ�� ��� ᫮�
// �.�. ��� ��ࠨ�� �������� ���ਬ�� "�������� ��i �ਢ�� 41 �������"

#if !defined(NUMERIC_AS_MONEY)
    // output: str="������� ��� �㡫� 41 �������"
    _TOOLEXP void LocalRubToStr(lmoney total, char *str);

    // output: str="������� ��� ������ 41 業�"
    _TOOLEXP void LocalCurToStr(lmoney  total,    // �㬬� � ������� ������
                                char   *str,      // c㬬� �ய����
                                int     CurCode   // ��� ������ (0-��樮���쭠� �����)
                               );

    // output: str="������� ��� ��. 41 ���."
    _TOOLEXP void rubtostr(lmoney total, char *str);

    // output: str="������� ��� .41 USD"
    _TOOLEXP void curtostr(lmoney  total,    // �㬬� � �㡫��
                           char   *str,      // c㬬� �ய����
                           char   *CurNam    // �������� ������ e.g. "USD"
                         );

    // output: str="������� ���", kop="41"
    _TOOLEXP void SumToStr(lmoney total, char *str, char *kop);

    _TOOLEXP void CurSumToStr(lmoney total, char *str, char *kop, int code);

    // ������⢮ �������� �।��⮢ �ய����
    // output: str="������� ��� ����"
    _TOOLEXP  void NumToStr(lmoney  total,
                            char   *str,
                            char   *name1,         // input: "����"
                            char   *name2,         // input: "����"
                            char   *name0,         // input: "���⮢"
                            int     Rod);          // input: 1=��᪮� த, 2=���᪨� த
#endif //!defined(NUMERIC_AS_MONEY)

// ������⢮ �������� �।��⮢ �ய����(�� � ��⮬ �ᥫ � ������饩 �窮�)
// output: str="������� ��� 楫�� �� ���� ����"
_TOOLEXP  void NumToStrEx(double total,
                          char *str,
                          char *name1,         // input: "����"
                          char *name2,         // input: "����"
                          char *name0,         // input: "���⮢"
                          int Rod,             // input: 1=��᪮� த, 2=���᪨� த
                          int Prec);           // �᫮ ������ ��᫥ ����⮩

// �������� �஡�� � ᫮�� wrd � ��ப� str
_TOOLEXP  void AddWord(char *str, char *wrd);

// ������� ����� �㪢� � �ப� str
_TOOLEXP  void Up1Letter(char *str);


_TOOLEXP char *MonthName(int   n,     // �����
                         char *mon);  // � ⥪�⮢�� ����

_TOOLEXP char *DayName(int   n,       // ����
                       char *day);    // � ⥪�⮢�� ����

_TOOLEXP char *YearName(char  *year); // � ⥪�⮢�� ����

#ifdef __cplusplus
}
#endif

// ==========================================================

#if defined(USE_NUMERIC)

    #ifdef __cplusplus

        _TOOLEXP void LocalRubToStr   // output: str="������� ��� �㡫� 41 �������"
        (
           const Numeric& total,
           char* str
        );

        _TOOLEXP void LocalRubToStrEx
        (
           const Numeric& total,
           char* str,
           int   prec                 // �᫮ ������ ��� ������
        );

        _TOOLEXP void LocalCurToStr   // output: str="������� ��� ������ 41 業�"
        (
           const Numeric& total,      // �㬬� � ������� ������
           char* str,                 // c㬬� �ய����
           int   CurCode              // ��� ������ (0-��樮���쭠� �����)
        );

        _TOOLEXP void LocalCurToStrEx
        (
           const Numeric& total,      // �㬬� � ������� ������
           char* str,                 // c㬬� �ய����
           int   curCode,             // ��� ������ (0-��樮���쭠� �����)
           int   prec                 // �᫮ ������ ��� ������
        );

        _TOOLEXP void rubtostr        // output: str="������� ��� ��. 41 ���."
        (
          const Numeric &total,
          char* str
        );

        _TOOLEXP void rubtostrEx      // output: str="������� ��� ��. 41 ���."
        (
           const Numeric &total,
           char* str,
           int   prec                 // �᫮ ������ ��� ������
        );

        _TOOLEXP void curtostr        // output: str="������� ��� .41 USD"
        (
           const Numeric &total,      // �㬬� � �㡫��
           char* str,                 // c㬬� �ய����
           char* CurNam               // �������� ������ e.g. "USD"
        );

        _TOOLEXP void SumToStr        // output: str="������� ���", kop="41"
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

        _TOOLEXP void SumToStrEx      // output: str="������� ���", kop="41"
        (
           const Numeric &total,
           char* str,
           char* kop,
           int   prec
        );

        // ������⢮ �������� �।��⮢ �ய����
        _TOOLEXP void NumToStr        // output: str="������� ��� ����"
        (
          const Numeric &total,
          char *str,
          char *name1,                // input: "����"
          char *name2,                // input: "����"
          char *name0,                // input: "���⮢"
          int Rod                     // input: 1=��᪮� த, 2=���᪨� த
        );

        // ������⢮ �������� �।��⮢ �ய����(�� � ��⮬ �ᥫ � ������饩 �窮�)
        _TOOLEXP  void NumToStrEx     // output: str="������� ��� 楫�� �� ���� ����"
        ( 
          const Numeric &total,
          char *str,
          char *name1,                // input: "����"
          char *name2,                // input: "����"
          char *name0,                // input: "���⮢"
          int Rod,                    // input: 1=��᪮� த, 2=���᪨� த
          int Prec                    // �᫮ ������ ��᫥ ����⮩
        );

    #endif // __cplusplus

#endif // defined(USE_NUMERIC)

// ==========================================================

#endif // __RUBTOSTR_H

/* <-- EOF --> */