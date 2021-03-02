//***********************************************************************
//                    �����㬥��਩ ��� ࠡ��� � dos-䠩����
//                            VECsoft 29.10.1992
//***********************************************************************
// October 25,1995   Serg Kubrin (K78)
//     Add CopyFileLong
// November 2,1995   Serg Kubrin (K78)
//     Add chunkNumber to CopyFileLong

#ifndef __DOSFILE_H
#define __DOSFILE_H

#include "bdate.h"
#include "apptldef.h"
#include "rsfname.h"

#ifdef __cplusplus
 extern "C" {
#endif

// ������� ��� 䠩�� �� ����
_APPTLFUNC void get_namefile(
                             char  *In,        // ���� � 䠩��
                             char  *Out,       // ��� 䠩��
                             bdate *Date       // ���
                            );

// ������� ��� 䠩�� � ���७��� � ��
_APPTLFUNC void get_extfile(
                            char *In,          // �室��� ��� 䠩�� ��� ���७��
                            char *Out          // ��室��� ��� 䠩�� � ���७���
                           );

// ������� ��� ࠡ�祣� 䠩��
_APPTLFUNC char *get_wrkfile(
                             const char *In,   // �室��� ��� 䠩�� ��� ���७�� � ���
                             char *Out         // ����� ���� � ��室���� 䠩��
                            );

// ����஢��� 䠩� � 䠩�
_APPTLFUNC int copyfile(
                        char *NameIn,     // ��� ��室���� 䠩��
                        char *NameOut,    // ��� �ਥ����� 䠩��
                        int   sure        // �ਧ��� ��易⥫쭮�� ����஢����
                       );


// -----------------------------------------------------------------------------
typedef enum
      {
       CP_NEXT_TARGET_FILE,
       CP_NEXT_SOURCE_FILE
      } TCopyFileOperation;

typedef struct
      {
       TCopyFileOperation  oper;
       char               *NameIn;         // ��� ��室���� 䠩��
       char               *NameOut;        // ��� 䠩�� �����祭��
       unsigned long       targetFreeSize; // ������ ᢮������� ����࠭�⢠ ��� 䠩��
                                           // �����祭�� ��� 0 �᫨ ����஫� �� �믮�����
       unsigned long       bytesWritten;   // ������⢮ ����ᠭ��� ����
       int                 chunkNumberIn;  // ����� ᥣ���� ��室���� 䠩��
       int                 chunkNumberOut; // ����� ᥣ���� 䠩�� �����祭��
      } TFileCopyInfo;


_APPTLFUNC unsigned long CopyFileLong(
                                      char *nameIn,                 // ��� ��室���� 䠩��
                                      char *nameOut,                // ��� 䠩�� �����祭��
                                      unsigned long sourceSize,     // ������ ��� ᥣ���⮢ ��室���� 䠫� ��� 0
                                                                    // �᫨ ᥣ���� ����
                                      unsigned long targetFreeSize, // ������ ᢮������� ����࠭�⢠ ��� 䠩��
                                                                    // �����祭�� �᫨ useFreeSize == True
                                      int (*userProc)(TFileCopyInfo *), // ��楤�� ����� �� ᬥ�� ��᪥��
                                                                    // �� �ᯥ� ������ �������� 0
                                      int useFreeSize               // �᫨ True �ᯮ�짮���� targetFreeSize
                                   );

#ifdef __cplusplus
 } ;
#endif

#endif // __DOSFILE_H

/* <-- EOF --> */