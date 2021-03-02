/*******************************************************************************
 FILE         :   RSAPPDIR.H

 COPYRIGHT    :   R-Style Software Lab, 1994, 1998

 DESCRIPTION  :

 PROGRAMMED BY:   Emil Dovidovitch

 CREATION DATE:   14 Mar 1994

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/rsappdir.h $
                  $Revision: 1 $
                  $Date: 19.10.98 19:26 $
                  $Author: Markovitch $
                  $Modtime: 16.10.98 21:28 $
*******************************************************************************/

#ifndef __RSAPPDIR_H
#define __RSAPPDIR_H

#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

// ����祭�� ����� 䠩�� � ⥪�饩 ��४�ਨ
_APPTLFUNC void getFileName(
                            char       *DirFileName,    // ��� 䠩�� � ��४�ਨ
                            const char *FileName        // ��� 䠩��
                           );

// ��ନ஢���� ������� ����� 䠩�� � ࠡ�祬 ��⠫��� (WORKFILE)
_APPTLFUNC void getWorkFileName(char *FullFileName, const char *SmallFileName);

// ��ନ஢���� ������� ����� 䠩�� � ��⠫��� <TXTFILE>
_APPTLFUNC void getTxtFileName(char *FullFileName, const char *SmallFileName);

// ��ନ஢���� ������� ����� 䠩�� � ��⠫��� <HELP>
void getHelpFileName(char *outfname, const char *fname);

// ��ନ஢���� ������� ����� 䠩�� � ��⠫��� <RES>
//void getResFileName(char *outfname, const char *fname);


_APPTLFUNC void MemFileDir(void);       // ���࠭��� �।��騩 ��४�਩
_APPTLFUNC void RestFileDir(void);      // ����⠭����� �।��騩 ��४�਩
_APPTLFUNC void SetTxtDir(void);        // ��⠭����� ��४�਩ ⥪�⮢�� 䠩���
_APPTLFUNC void SetWorkDir(void);       // ��⠭����� ��४�਩ ⥪�⮢�� 䠩���
_APPTLFUNC void SetObjDir(void);        // ��⠭����� ��४�਩ exe 䠩���
_APPTLFUNC void SetToolsDir(void);      // ��⠭����� ��४�਩ tools 䠩���
_APPTLFUNC void SetDBfileDir(void);
_APPTLFUNC void SetResFileDir(const char* resFileDir);
_APPTLFUNC void SetHelpFileDir(const char* helpFileDir);

#ifdef __cplusplus
}
#endif

#endif // __RSAPPDIR_H

/* <-- EOF --> */