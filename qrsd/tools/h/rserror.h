// -------------------------------------------------------------------------------------------------
// 2013-03-29  �����㬥�� ��� �ନ஢���� ����� �訡��, ᮢ���⨬�� �� �ଠ�� � ������
//             �訡�� Microsoft. �� �������� �ᯮ�짮���� �㭪樨 ����
//             SetLastError/GetLastError ��� ����⨢��� ��᫥��⢨�.

#ifndef _RSERROR_H_
#define _RSERROR_H_


// -----------------------------------------------------------------------------
// ����஢���� �訡�� � �ଠ� Microsoft (32 ���)

//  ����            �����祭��
//
//  0 - 15    ��� �訡�� (0 - 65535)
// 16 - 27    ��� �����⥬� (0 - 4095)
// 28         ����� (������ ���� 0)
// 29         �ਭ���������� �訡��:
//                0 - Microsoft
//                1 - ���짮��⥫�᪠�
// 30 - 31    ��� �訡��:
//                0 - �ᯥ�
//                1 - ���ଠ��
//                2 - �।�०�����
//                3 - �訡��

// �.�. ��� �ᯮ�짮����� �������筮�� �ଠ� � �����㬥�⠫쭮�/�ਪ������ ���� �㦭�
// �믮����� ���� �᫮���: ��� 29 ������ ���� ��⠭����� � 1.
//
// ��� �����⥬� (���� 16 - 27) �।���������� ࠧ������ �� ��� ������������:
//       0 -  511  - �����㬥��
//     512 - 4095  - �ਪ������ ���
//
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
//
//
// ��� �����䨪��� �����⥬� �ਪ������� ���� � ����⢥ ��ࢮ�� �ਡ�������
// ����� ����� 1000 + �����᫥��멊��������������⥬�.


// -----------------------------------------------------------------------------

// ��᪠ ��� ����祭�� ��⥣�ਨ �訡��
#define  RSERR_CATEGORY                 0x20000000

// ��᪠ ��� ����祭�� ���� �訡��
#define  RSERR_ERROR_MASK               0x0000FFFF

// ��᪠ ��� ����祭�� ���� �����⥬�
#define  RSERR_SUBSYSTEM_MASK           0x0FFF0000

// ��᪠ ��� �஢�ન �� �ਭ���������� ���� �����⥬� �ਪ������� ����
#define  RSERR_SUBSYSTEM_USER_MASK      0x0E000000

// ��᪠ ��� ����祭�� ⨯� �訡��
#define  RSERR_TYPE_MASK                0xC0000000


// -----------------------------------------------------------------------------
// ����⠭�� ��� ⨯� �訡��
#define  RSERR_TYPE_OK           0x00  // �ᯥ�
#define  RSERR_TYPE_INFORMATION  0x01  // ���ଠ��
#define  RSERR_TYPE_WARNING      0x02  // �।�०�����
#define  RSERR_TYPE_ERROR        0x03  // �訡��


// -----------------------------------------------------------------------------

// ��।�����, ���� �� �訡�� "��襩" (�.�. �����㬥�⠫쭮� ��� �ਪ������ -
// ����饬, �� ���� �訡��� Microsoft) �� 29-�� ����.
#define  RSERR_IsOurError(err)            ((err) & RSERR_CATEGORY)

// �������-�� �訡�� �����㬥�⠫쭮�
#define  RSERR_IsToolsError(err)          ((err) & RSERR_CATEGORY) && !((err) & RSERR_SUBSYSTEM_USER_MASK))

// �������-�� �訡�� �ਪ������
#define  RSERR_IsUserError(err)           ((err) & RSERR_CATEGORY) && ((err) & RSERR_SUBSYSTEM_USER_MASK))


// �������/��⠭����� ��� �訡�� (���祭�� � ���� 0 - 15)
#define  RSERR_GetError(err)              ((err) & RSERR_ERROR_MASK)
#define  RSERR_SetError(err, code)        ((err) | ((code) & RSERR_ERROR_MASK))

// �������/��⠭����� ��� �����⥬� (���祭�� � ���� 16 - 27)
#define  RSERR_GetSubsystem(err)          (((err) & RSERR_SUBSYSTEM_MASK) >> 16)
#define  RSERR_SetSubsystem(err, code)    ((err) | (((code) << 16) & RSERR_SUBSYSTEM_MASK))

// �������/��⠭����� ⨯ �訡�� (���祭�� � ���� 30 - 31).
// � ����⢥ type ����� �ᯮ�짮������ ����⠭�� ���� RSERR_TYPE_.
#define  RSERR_GetErrorType(err)          (((err) & RSERR_TYPE_MASK) >> 30)
#define  RSERR_SetErrorType(err, type)    ((err) | (((type) << 30) & RSERR_TYPE_MASK))


// ��ନ஢��� ������ଠ��� �訡�� �� ᫥����� ���������:
//    type - ⨯ �訡��      (���� 30 - 31)
//    ss   - ��� �����⥬�  (���� 16 - 27)
//    code - ��� �訡��      (���� 0 - 15)
#define  RSERR_MakeFull(type, ss, code)   (RSERR_CATEGORY | ((((type) << 30) & RSERR_TYPE_MASK) | (((ss) << 16) & \
                                           RSERR_SUBSYSTEM_MASK) | ((code) & RSERR_ERROR_MASK)))

// ��ନ஢��� �訡�� � ������� ����� ��� �������� �����⥬�
#define  RSERR_MakeError(ss, code)        RSERR_MakeFull(RSERR_TYPE_ERROR, (ss), (code))

// ��ନ஢��� ���ଠ樮���� ᮮ�饭�� � ������� ����� ��� �������� �����⥬�
#define  RSERR_MakeInformation(ss, code)  RSERR_MakeFull(RSERR_TYPE_INFORMATION, (ss), (code))

// ��ନ஢��� �।�०����� � ������� ����� ��� �������� �����⥬�
#define  RSERR_MakeWarning(ss, code)      RSERR_MakeFull(RSERR_TYPE_WARNING, (ss), (code))

// ��ନ஢��� �ᯥ譮� ᮮ�饭�� � ������� ����� ��� �������� �����⥬�
#define  RSERR_MakeOk(ss, code)           RSERR_MakeFull(RSERR_TYPE_OK, (ss), (code))

#endif // _RSERROR_H_

/* <-- EOF --> */