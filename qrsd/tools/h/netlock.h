//***********************************************************************
//   Group locks for NetWare applications.
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/netlock.h $
//                  $Revision: 7 $
//                  $Date: 20.03.00 20:11 $
//                  $Author: Lebedev $
//                  $Modtime: 17.03.00 14:24 $
//***********************************************************************

// October 13,1994     ��ਭ �. (K78)
//               ��ࠫ ����祭�� nwcalls.h
// 22.10.99 ��������� ��ॢ����� �� ����ୠ⨢��� ॣ������

#ifndef __NETLOCK_H
#define __NETLOCK_H

#include "apptldef.h"
#include "fs.h"

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
typedef void (* SAddrFunc_t)(unsigned char *);

extern _APPTLDATA SAddrFunc_t  SAddrFuncPtr;  // �����⥫� �� �㭪�� ����祭�� ���� 

extern _APPTLDATA unsigned int   CHandle;
extern _APPTLDATA unsigned int   CNum;
extern _APPTLDATA unsigned char  networkNumber[4];
extern _APPTLDATA unsigned char  SAddr[6];
extern _APPTLDATA unsigned int   socketNumber;


// -----------------------------------------------------------------------------
// ��⠭����� ⥪�騥 �⥢� ��ࠬ����
// �᫨ � ����⢥ ��ࠬ��� data ��।��� NULL, �⥢� ��ࠬ���� ������ ����
// ����襭� � ������� �㭪樨 fs_getTerminalNetInfo.
// [in]  data - 㪠��⥫� �� ��������, ᮤ�ঠ��� ���� ���祭�� ��ࠬ��஢
_APPTLFUNC void SetClientNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// ��⠭����� ⥪�騥 �⥢� ��ࠬ����
// �᫨ � ����⢥ ��ࠬ��� data ��।��� NULL, �⥢� ��ࠬ���� ������ ����
// ����襭� � ������� �㭪樨 fs_getTerminalNetInfo2.
// [in]  data - 㪠��⥫� �� ��������, ᮤ�ঠ��� ���� ���祭�� ��ࠬ��஢
_APPTLFUNC void SetClientNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// ������� ⥪�騥 �⥢� ��ࠬ����
// [out]  data - 㪠��⥫� �� ��������-�����祭��
_APPTLFUNC void GetClientNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// ������� ⥪�騥 �⥢� ��ࠬ����
// [out]  data - 㪠��⥫� �� ��������-�����祭��
_APPTLFUNC void GetClientNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// ������� ������� �⥢� ��ࠬ����
// [out]  data - 㪠��⥫� �� ��������-�����祭��
_APPTLFUNC void GetLocalNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// ������� ������� �⥢� ��ࠬ����
// [out]  data - 㪠��⥫� �� ��������-�����祭��
_APPTLFUNC void GetLocalNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// ��������� ࠡ��� �ਫ������ � ��⥬� (� ���� ������)
// ���祭�� ��ࠬ��� grn = "OFF" �⬥��� ॣ������
_APPTLFUNC void initNetwGr    // Network group initialisation.
                          (
                           char *grn                // Group name. Truncated to 8 characters!
                          );


// �㭪�� ࠧॣ����樨. ��뢠�� ��� ��������� �� ����, ��᪮��� ��� �⠭������
// atexit'���� �㭪樥� �� �믮������ initNetwGr
_APPTLFUNC void ClearConnection(void);


// ����祭�� ��� ��⨥ �������쭮�� ����㯠 (GrLock( OFF ) �������⭮
// �맮�� ClearGrLock
_APPTLFUNC unsigned char GrLock(                         // 0 - OK  1 - Failed  2 - Network error
                                unsigned char excl       // ON - Full lock needed
                               );

// ���⨥ �������쭮� ࠡ��� (ࠧ�����஢���� ��㯯�)
_APPTLFUNC void ClearGrLock(void);

// �������� �������騥 ���ᮥ�������
_APPTLFUNC void ShowConnection(void);

// �������� ᯨ᮪ ���ᮥ�������
_APPTLFUNC void ReloadConnection(void);

#ifdef __cplusplus
}
#endif

#endif // __NETLOCK_H

/* <-- EOF --> */