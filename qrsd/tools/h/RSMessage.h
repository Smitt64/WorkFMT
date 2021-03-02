// -------------------------------------------------------------------------------------------------
// 2010-12-06  ����䥩� ��� ࠡ��� � ��⥬�묨 (�����㬥�⠫�묨) ᮮ�饭�ﬨ

#ifndef _RSSYSTMESSAGE_H_
#define _RSSYSTMESSAGE_H_

#include "platform.h"
#include "tooldef.h"


// -----------------------------------------------------------------------------
// ����⪠ ��� �����祭�� �ᯮ�짮����� RSSendMessage
#define  dRSSendMessageEx(msg, data, add, flag)  RSSendMessage(CRSMessage((msg), (data), (add)), (flag))
#define  dRSSendMessage(msg, data, add)          RSSendMessage(CRSMessage((msg), (data), (add)), false)

// -----------------------------------------------------------------------------
// �����ন����� ᮮ�饭��
enum TERSMessage
   {
    eMsg_None           = -2,      // ��㦥��� ⨯ ᮮ�饭�� (���������)
    eMsg_All            = -1,      // ��㦥��� ⨯ ᮮ�饭�� (��)

    eMsg_Zero           = 0,       // �㫥��� ᮮ�饭�� (�㦥��� १��)

    eMsg_Allocate,                 // �뤥����� ����� � ������� Allocate
    eMsg_FreeMem,                  // �᢮������� ����� � ������� FreeMem

    eMsg_ReadyInput,               // ������ ᨬ���� "R", �뢮������ � EasyWin-०��� �� ��⮢���� ��⥬� � �����

    eMsg_WaitKeyboardBegin,        // �������� ����� � ��������� ��� ���
    eMsg_WaitKeyboardEnd,          // �����襭�� �������� ����� � ���������� ��� ���

    eMsg_RSMessageDone,            // �����樠������ ��堭���� ᮮ�饭��

    eMsg_PlayRep        = 500,     // �믮������ ����䠩��
    eMsg_RunMacro,                 // �맮� �����
    eMsg_PlayRepEnd,               // �����襭�� �믮������ ����䠩��
    eMsg_RunMacroEnd,              // �����襭�� �믮������ �����

    // -------------------------------------------
    eMsg_ToolsInterface = 5000,    // ����� ���� ����䥩�� ᮮ�饭�� �����㬥��

    eMsg_BeforeShowPanel,          // �뤠���� �����।�⢥��� ��। �⮡ࠦ����� ������ �� �࠭�
    eMsg_AfterShowPanel,           // �뤠���� �ࠧ� ��᫥ �⮡ࠦ���� ������ �� �࠭�
    eMsg_BeforeRemovePanel,        // �뤠���� �����।�⢥��� ��। 㤠������ ������ � �࠭�
    eMsg_AfterRemovePanel,         // �뤠���� �ࠧ� ��᫥ 㤠����� ������ � �࠭�

    eMsg_TestHorMenu,              // �뤠���� �� �맮�� TestHM
    eMsg_SelectHorMenu,            // �뤠���� �� �롮� ���짮��⥫�

    eMsg_cwinExBegin,              // �뤠���� �� �맮�� cwinEx
    eMsg_cwinExEnd,                // �����頥� �롮� ���짮��⥫�

    eMsg_procfldBegin,             // �뤠���� �� �맮�� procfld
    eMsg_procfldEnd,               // �����頥� �롮� ���짮��⥫� (�� ��뢠���� ��। PanProc)

    eMsg_EditFileBegin,            // �뤠���� ��। �뢮��� ।����
    eMsg_EditFileEnd,              // �뤠���� ��। �����⨥� ।����

    eMsg_procfldEndReal,           // ������� eMsg_procfldEnd

    eMsg_TestVertMenu,             // �뤠���� �� �맮�� LoopM
    eMsg_SelectVertMenu,           // �뤠���� �� �롮� ���짮��⥫�

    // -------------------------------------------
    eMsg_User           = 10000    // � �⮣� �����䨪��� ��稭����� �������� ���짮��⥫�᪨� ᮮ�饭��
   };

// -----------------------------------------------------------------------------
// ��� ��������᪮�� 㤮��⢠
#define  eMsg_PlayRepBegin   eMsg_PlayRep
#define  eMsg_RunMacroBegin  eMsg_RunMacro

// -----------------------------------------------------------------------------
// �।��।������ ������ ������
enum TERSMessageStatus
   {
    eMsgStatus_Blocked                = -5,  // ��ࠡ��稪 �������஢�� � ������� RSControlMessageBlockMode()
    eMsgStatus_NoUserRecursionHandler = -4,  // ��������� ���짮��⥫�᪨� ��ࠡ��稪 ४��ᨢ��� ᮮ�饭��
    eMsgStatus_LimitRecursion         = -3,  // �ॢ�襭 ����� ��������� ᮮ�饭��
    eMsgStatus_Disable                = -2,  // ��堭��� ᮮ�饭�� ����⨢��
    eMsgStatus_NoHandler              = -1,  // ��������� ��ࠡ��稪 ��� ������� ᮮ�饭��
    eMsgStatus_Ok                     =  0,  // ��� �ᥣ� "�ᯥ�"
    eMsgStatus_NeedChange             =  1   // ��ࠡ��稪 ������� ��室��� ���祭��
   };

// -----------------------------------------------------------------------------
// ������ �����஢�� ᮮ�饭��
enum TERSMessageBlockMode
   {
    eMsgBlockMode_None                = 0,   // �����஢�� ���������
                                             // ���祭�� 1 � 2 ��१�ࢨ஢���
    eMsgBlockMode_Interface           = 4,   // �����஢�� ����䥩��� ᮮ�饭�� ([5000; 9999])
    eMsgBlockMode_User                = 8    // �����஢�� ���짮��⥫�᪨� ᮮ�饭�� (� 10000 � ���)
   };

// -----------------------------------------------------------------------------
// ������� �������⥫��� ������ ��� eMsg_PlayRep/eMsg_PlayRepEnd
typedef struct TSMsgPlayRep
      {
       int          type;     // ��� �맮�� (���� �� �ᯮ������)
       const char  *output,   // ���� �����
                   *nspace;   // ����࠭�⢮ ���
       int          stat;     // ��� ᮮ�饭�� eMsg_PlayRepEnd ᮤ�ন� ���祭�� STD_USERDATA::stat
      } TSMsgPlayRep;

// -----------------------------------------------------------------------------
// �᭮���� ������� ��� ��ࠬ��� ᮮ�饭�� eMsg_WaitKeyboardBegin/eMsg_WaitKeyboardEnd
typedef struct TSMsgKeyboard
      {
       int   version;         // ����� �������� (���� 1)
       int   key,             // ��� ������
             ungkey;          // �������� ��� ������
       int   scan,            // ���-��� ������
             flags;           // ����� ������
      } TSMsgKeyboard;

// -----------------------------------------------------------------------------
// �᭮���� ������� ��� ��ࠬ��� ᮮ�饭�� eMsg_cwinExBegin/eMsg_cwinExEnd
typedef struct TSMsgCWinEx
      {
       int           version;         // ����� �������� (���� 1)
       const char  **texts,           // ���ᨢ ��ப ᮮ�饭��
                   **buttons;         // ���ᨢ �������� ������
       int           nTexts,          // ������ ���ᨢ� ��ப ᮮ�饭��
                     nButtons;        // ������ ���ᨢ� �������� ������
       int           currButton;      // ������ ��-㬮�砭��
       const char   *head,            // ��������� ᮮ�饭��
                    *status;          // �����-��ப� ᮮ�饭��
       int           infoType;        // ?
       long          errCode;         // ?
      } TSMsgCWinEx;

// -----------------------------------------------------------------------------
// �᭮���� ������� ��� ��ࠬ��� ᮮ�饭�� eMsg_EditFileBegin/eMsg_EditFileEnd
typedef struct TSMsgEditFile
      {
       int           version;         // ����� �������� (���� 1)
       const char   *file,            // ��� (+����) � ���뢠����� 䠩��
                    *output,          // ��室��� 䠩�
                    *head;            // ���������
       int           mode;            // ����� ।���� (�. ����⠭�� � ��䨪ᮬ V_ ��
                                      // rstools\tools\h\edit\viewed.h)
      } TSMsgEditFile;


// -----------------------------------------------------------------------------
// ����� ������ ᮮ�饭��
class _TOOLEXP CRSMessage
    {
     private :
       TERSMessage  m_pv_Message,        // ����饭��
                    m_pv_ParentMessage;  // ����⥫�᪮� ᮮ�饭�� (��⠭���������� ��� ��ࠡ��稪� ��� ᮮ�饭��)
       void        *m_pv_Data,           // ����� ᮮ�饭��
                   *m_pv_AddData;        // �������⥫�� ����� ᮮ�饭��

       // ----------------------------------------
       void _Init(TERSMessage msg = eMsg_None, TERSMessage parent = eMsg_None, void *data = NULL, void *add = NULL);

     public :
       // ----------------------------------------
       CRSMessage(TERSMessage msg, void *data = NULL, void *add = NULL);
       CRSMessage(TERSMessage msg, TERSMessage parent, void *data = NULL, void *add = NULL);

       // ----------------------------------------
       // ������� �����䨪��� ᮮ�饭��
       TERSMessage Message(void) const;

       // ������� �����䨪��� த�⥫�᪮�� ᮮ�饭��
       TERSMessage ParentMessage(void) const;

       // ----------------------------------------
       // ������� ����� ᮮ�饭��
       void *Data(void) const;

       // ----------------------------------------
       // ������� �������⥫�� ����� ᮮ�饭��
       void *AddData(void) const;
    };

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ����祭��\�몫�祭�� ��堭���� ᮮ�饭��
// [in]  flagEnable - ����� ���祭�� 䫠��
// [ret]            - �����頥� �।��饥 ���ﭨ� 䫠��
_TOOLFUNC bool RSControlMessage(bool flagEnable);

// -----------------------------------------------------------------------------
// ��ࠢ����� ०���� �����஢�� ᮮ�饭��
// [in]  mode - ����� ���祭�� ०��� (�. TERSMessageBlockMode)
// [ret]      - �����頥� �।��饥 ���ﭨ� ०���
int RSControlMessageBlockMode(int mode);

// -----------------------------------------------------------------------------
// ���樠������ ��堭���� ᮮ�饭��
int RSMessageInit(void);

// -----------------------------------------------------------------------------
// �����樠������ ��堭���� ᮮ�饭��
void RSMessageDone(void);

// -----------------------------------------------------------------------------
// ��ࠢ�� ᮮ�饭��
// [in]    msg     - ᮮ�饭��
// [in]    noBlock - �ਭ㤨⥫쭠� ��ࠢ�� ᮮ�饭�� (��ᬮ��� �� �����஢��)
// [ret] - �����頥�� ���祭��:
//          0 -
//         -1 - ��ࠡ��稪 ��� ������� ᮮ�饭�� �� �� ������
//         -2 - ��堭��� ᮮ�饭�� ����⨢��
//         -3 - �ॢ�襭 ����� ��������� ᮮ�饭��
//         -5 - ᮮ�饭�� �������஢���
_TOOLFUNC int RSSendMessage(const CRSMessage &msg, bool noBlock = false);


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ��� - 㪠��⥫� �� �㭪��-��ࠡ��稪 ᮮ�饭��
// [in]      msg - ᮮ�饭��
// [ret] - �����頥�� ���祭��:
//         0 - ᮮ�饭�� �뫮 �ᯥ譮 ��ࠡ�⠭�, ��⠫�� ��ࠡ��稪� �� ��뢠��
//        -1 - �맢��� ᫥���騩 ��ࠡ��稪 � 楯�窥
typedef int (*TFRSMessageHandler)(const CRSMessage &msg);

// -----------------------------------------------------------------------------
// ���������� ��ࠡ��稪� � ᯨ᮪
// [in]  hMsg - 奭�� ��ࠡ��稪�
// [in]  msg  - �����䨪��� ᮮ�饭�� (�᫨ � ����⢥ msg �㤥� ��।�� ⨯
//              eMsg_All, ��ࠡ��稪 �㤥� ������� �� ᮮ�饭��)
// [ret] - �����頥�� ���祭��:
//         true  - ��ࠡ��稪 �ᯥ譮 ��������
//         false - �訡�� ���������� ��ࠡ��稪�
_TOOLFUNC bool RSAddMessageHandler(TFRSMessageHandler hMsg, TERSMessage msg);

// -----------------------------------------------------------------------------
// �������� ��ࠡ��稪� �� ᯨ᪠
// [in]  hMsg - 奭�� ��ࠡ��稪� (�᫨ � ����⢥ hMsg �㤥� ��।�� NULL,
//              ���� 㤠���� �� ��ࠡ��稪�)
// [in]  msg  - �����䨪��� ᮮ�饭�� (�᫨ � ����⢥ msg ��।��� ᨬ�����᪠�
//              ����⠭� eMsg_None, � ��ࠡ��稪 �㤥� �᪫�祭 �� ��� ᮮ�饭��)
// [ret] - �����頥�� ���祭��:
//         true  - ��ࠡ��稪 �ᯥ譮 㤠���
//         false - �訡�� 㤠����� ��ࠡ��稪�
_TOOLFUNC bool RSRemoveMessageHandler(TFRSMessageHandler hMsg, TERSMessage msg);

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// �����⥫� �� ��ࠡ��稪 ४��ᨢ��� ᮮ�饭��
// ���祭��, �����頥��� ��ࠡ��稪�� ४��ᨢ��� ᮮ�饭��, ���� �� �ᯮ������.
typedef  TFRSMessageHandler  TFRecursionMessageHandler;

// -----------------------------------------------------------------------------
// ��⠭����� ��ࠡ��稪 ४��ᨢ��� ᮮ�饭��
// [in]  newMsg - 㪠��⥫� �� ���� ��ࠡ��稪 ᮮ�饭��
// [ret]        - �����頥� 㪠��⥫� �� �।��騩 ��ࠡ��稪
_TOOLFUNC TFRecursionMessageHandler RSSetRecursionMessageHandler(TFRecursionMessageHandler newMsg);

// -----------------------------------------------------------------------------
// ��⠭����� ���ᨬ��쭮� ������⢮ ��������� ᮮ�饭�� ������ ����
// [in]  newVal - ����� ���ᨬ��쭮� ���祭��
// [ret]        - ������ �।��饥 ���ᨬ��쭮� ���祭��
_TOOLFUNC int RSSetMaxRecursionCount(int newVal);

// -----------------------------------------------------------------------------

#endif // _RSSYSTMESSAGE_H_

/* <-- EOF --> */