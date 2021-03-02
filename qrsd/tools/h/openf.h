/************************************************************************
            ���ࠪ⨢�� �롮� 䠩���
            VECsoft 10.11.92

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/openf.h $
                  $Revision: 9 $
                  $Date: 1.06.00 18:57 $
                  $Author: Lebedev $
                  $Modtime: 27.05.00 20:33 $
************************************************************************/

#ifndef __OPENF_H
#define __OPENF_H

#include <pexpdef.h>
#include "rsfname.h"


// -----------------------------------------------------------------------------
// ������ �⡮� 䠩���
#define  OPEN_FILE_LOCAL     0  // �����쭮
#define  OPEN_FILE_REMOTE    1  // ��������

// ������ ���஢�� 䠩��� � �஫����� �롮�
#define  SORT_FILE_BY_NAME   0  // �� �����
#define  SORT_FILE_BY_EXT    1  // �� ���७��
#define  SORT_FILE_BY_DT     2  // �� ���/�६���
#define  SORT_FILE_BY_SIZE   3  // �� ࠧ����

#ifdef __cplusplus
extern "C" {
#endif

_RSPNLEXP int  Openf_Constr // �뤥���� ������, �����⮢��� �����
                           (
                            char *SaveLDir   // ���� ��� ��࠭���� 䠩�� � ��ॢ�� ��⠫����
                                             // NULL - � ��୥��� ��⠫���
                                             // �ਬ�� "c:\\user\\curdir"
                           );

_RSPNLEXP int  Openf_Loop   // �롮� 䠩��
                         (
                          const char *title,     // ���������
                          const char *ext,       // ���७��
                          char       *filename   // ��� ��࠭���� 䠩��
                         );

_RSPNLEXP int OpenFileWin_LoopSort  // return 0 - O'K  -1 Cansel  > 0 Numb MarkFiles
                                  (
                                   const char *title,     // ?????????
                                   const char *mask,      // �?���??�
                                   char       *filename,        // �?� ?�?�?????? �????
                                   int         sortBy
                                  );

_RSPNLEXP void Openf_Destr(void);  // �᢮������ ������

_RSPNLEXP int OpenFileWin_Constr(void);  // �뤥���� ������ - �����⮢��� �����
_RSPNLEXP int OpenFileWin_Loop  // return 0 - O'K  -1 Cansel  > 0 Numb MarkFiles
                              (
                               const char *title,     // ���������
                               const char *mask,      // ���७��
                               char       *filename   // ��� ��࠭���� 䠩��
                              );

_RSPNLEXP void OpenFileWin_Destr(void);     // �᢮������ ������


// -----------------------------------------------------------------------------
// �롮� 䠩�� ��� ��⠫��� (�����쭮)
// [in]  title    - ��������� �������
// [in]  mask     - ��᪠ �롮� 䠩���
// [out] filename - ��࠭�� 䠩�
// [ret]            0    - ��࠭ ������� 䠩�/��⠫��
//                  HOME - ��࠭� ��᪮�쪮 䠩���/��⠫���� (�⬥⪠ �� INS)
//                  ��⠫�� ���祭�� ������������ ��� �⪠� �� �롮�.
_RSPNLEXP int OpenFileWin(const char *title, const char *mask, char *filename);

// -----------------------------------------------------------------------------
// �롮� 䠩��
// [in]  title    - ��������� �������
// [in]  mask     - ��᪠ �롮� 䠩���
// [out] filename - ��࠭�� 䠩�
// [in]  opts     - ��樨 (���� OPEN_FILE_*)
// [ret]            0    - ��࠭ ������� 䠩�
//                  HOME - ��࠭� ��᪮�쪮 䠩��� (�⬥⪠ �� INS)
//                  ��⠫�� ���祭�� ������������ ��� �⪠� �� �롮�.
_RSPNLEXP int SelectFile(const char *title, const char *mask, char *filename, int opts);

// -----------------------------------------------------------------------------
// �롮� 䠩�� � �������� ���஢�� ᯨ᪠
// [in]  title    - ��������� �������
// [in]  mask     - ��᪠ �롮� 䠩���
// [out] filename - ��࠭�� 䠩�
// [in]  opts     - ��樨 (���� OPEN_FILE_*)
// [in]  sortBy   - ����� ���஢�� (���� SORT_FILE_BY_*)
// [ret]            0    - ��࠭ ������� 䠩�
//                  HOME - ��࠭� ��᪮�쪮 䠩��� (�⬥⪠ �� INS)
//                  ��⠫�� ���祭�� ������������ ��� �⪠� �� �롮�.
_RSPNLEXP int SelectFileSort(const char *title, const char *mask, char *filename, int opts, int sortBy);

// -----------------------------------------------------------------------------
// �롮� 䠩�� � �������� ���஢�� ᯨ᪠ � ������������ �롮� ����஢��
// [in]  title    - ��������� �������
// [in]  mask     - ��᪠ �롮� 䠩���
// [out] filename - ��࠭�� 䠩�
// [in]  opts     - ��樨 (���� OPEN_FILE_*)
// [in]  sortBy   - ����� ���஢�� (���� SORT_FILE_BY_*)
// [out] encPtr   - ����஢�� (enum UFEncodeType)
// [ret]            0    - ��࠭ ������� 䠩�
//                  HOME - ��࠭� ��᪮�쪮 䠩��� (�⬥⪠ �� INS)
//                  ��⠫�� ���祭�� ������������ ��� �⪠� �� �롮�.
_RSPNLEXP int SelectFileEncode(const char *title, const char *mask, char *filename, int opts,
                               int sortBy, int *encPtr);

// -----------------------------------------------------------------------------
// �롮� ��⠫���
// [in]  title    - ��������� �������
// [in]  path     - ��⠫�� ��-㬮�砭��
// [out] dirname  - ��࠭�� ��⠫��
// [in]  opts     - ��樨 (���� OPEN_FILE_*)
// [ret]            0    - ��࠭ ������� ��⠫��
//                  HOME - ��࠭� ��᪮�쪮 ��⠫���� (�⬥⪠ �� INS)
//                  ��⠫�� ���祭�� ������������ ��� �⪠� �� �롮�.
_RSPNLEXP int SelectFolder(const char *title, const char *path, char *dirname, int opts);

// -----------------------------------------------------------------------------
// �஢�ઠ ��᪨ �� ᮮ⢥��⢨� ��࠭�祭��
// [in]  curm - �஢��塞�� ��᪠
// [ret]
int TestRestrict(const char *curm);

// -----------------------------------------------------------------------------
// �஢���� ���४⭮��� ��᪨ � ��⠭����� �� � ����⢥ ⥪�饩
// ����砥� � ᥡ� ⠪�� �맮� �㭪樨 TestRestrict.
// [in]  curm - �஢��塞�� ��᪠
// [ret]        0 - �ᯥ�
//             !0 - �訡�� (�� ��諠 �஢�ઠ TestRestrict)
int CheckMask(char *curm);

// -----------------------------------------------------------------------------
// ����� 䠩���
int ShowFileHistory(int x1, int y1, int x2, int y2, char *str);

// -----------------------------------------------------------------------------
// ������塞 䠩� � �����
int AddFileToHistory(bdate *dtn, btime *tmn, const char *file);

// -----------------------------------------------------------------------------
// ���樠������ ��堭���� ���ਨ
int FileHistoryInit(void);

// -----------------------------------------------------------------------------
// �����樠������ ��堭���� ���ਨ
void FileHistoryDone(void);


// -----------------------------------------------------------------------------
// ������� �⬥祭�� 䠩�
// ��� ����祭�� ��� �⬥祭��� 䠩��� �㭪�� ����� ��뢠���� � 横�� �� �� ���,
// ���� �����頥��� ���祭�� ࠢ�� 0.
// ��������! �� ����祭�� 䠩��� �� ᯨ᪠ �ந�室�� ����䨪��� ��᫥�����, �᫥��⢨�
//           祣� ����୮� ����祭�� �⬥祭��� 䠩��� ����������!
// [out] filename - �⬥祭�� 䠩� � ��⥬
// [ret] � ��砥 �ᯥ� �����頥� 0 (� ���� filename ����ᠭ �⬥祭�� 䠩�)
//       ��� �᫮ > 0 (ᯨ᮪ �⬥祭��� 䠩��� ����).
_RSPNLEXP int GetMarkFile(char *filename);

// -----------------------------------------------------------------------------
// ������� �᫮ �⬥祭��� 䠩���/��⠫����
_RSPNLEXP int NumbMarkFile(void);

// -----------------------------------------------------------------------------
// ������� ��⠫�� ��࠭���� 䠩��
// ������⮬ �㭪樨 ���� ��⠫�� ��࠭���� �����筮�� 䠩��. � ��⠫��� �����
// �����頥��� ���祭�� ��᫠ �� �����.
_RSPNLEXP const char *RslGetLastSelectedDir(void);

#ifdef __cplusplus
}
#endif

#endif // __OPENF_H

/* <-- EOF --> */