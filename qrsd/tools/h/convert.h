// ��������:     RS-TOOLS 3.0 (c) VECsoft 1992-1993, (c) R-Style 1993.
// �����:       �८�ࠧ���⥫�� ������.
// �ணࠬ����:  ����� ���������.
// ���ଠ��:
// �����:      14/01/94
//

#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <tooldef.h>

#ifdef __cplusplus
 extern "C" {
#endif


int iCnvDBtxtToBtype(
                     char         *sField,
                     char          cType,           // type of data  'C', 'N', 'D', 'L', 'M'.
                     unsigned int  uiWidth,         // field width
                     unsigned char ucDec,           // length of decimal places
                     void         *pV,              // ���祭��
                     unsigned int  uiBType,         // ��� ���祭��
                     unsigned char ucP              // ������ ��᫥ �窨 ��� ���ᨬ��쭠� �����
                    );

_TOOLEXP char *szTermDir(char *Dir);

#ifdef __cplusplus
 } ;
#endif

#endif // _CONVERT_H_

/* <-- EOF --> */