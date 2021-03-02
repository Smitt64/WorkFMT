// -------------------------------------------------------------------------------------------------
// 2013-04-03  �����䨪���� �����㬥�⠫��� �����⥬, �ᯮ��㥬� ��� �ନ஢����
//             �訡�� (rserror.h).

#ifndef _RSSYSTID_H_
#define _RSSYSTID_H_

// -----------------------------------------------------------------------------
// ��� �����㬥�⠫쭮�� ������������ �।���������� ᫥���饥 ��।������:
//       0         - ��饨����㬥�⠫쭠� �訡��
//      10 -   19  - ���������� rstools
//      20 -   29  - ���������� rsscript (� �.�. � RSL)
//      30 -   49  - ���������� rscomserv
//      50 -   69  - ���������� DLM
//      70 -   79  - ���������� rsadmin
//      80 -   99  - ���������� utils
//     100 -  139  - ���������� server (�� � ��ନ���, �᪫��� �⨫��� ���������஢����)
//     500 -  511  - १�� ��� ���७��


// -----------------------------------------------------------------------------
// ���������� rstools
#define  RSTID_RSTools_Tools      10
#define  RSTID_RSTools_Panel      11
#define  RSTID_RSTools_Bedit      12

// -----------------------------------------------------------------------------
// ���������� �� � ��ନ����
#define  RSTID_AppServ_AppSrv    100      // ��ࢥ� �ਫ������
#define  RSTID_AppServ_6678t     101
#define  RSTID_AppServ_DesKey    102
#define  RSTID_AppServ_RComCnt   103
#define  RSTID_AppServ_Rsc       104
#define  RSTID_AppServ_Config    105
#define  RSTID_AppServ_TstHsp    106

#define  RSTID_AppServ_Adm       120
#define  RSTID_AppServ_PngClnt   121
#define  RSTID_AppServ_Rsm       122
#define  RSTID_AppServ_Term      123      // ��ନ���

#endif // _RSSYSTID_H_

/* <-- EOF --> */