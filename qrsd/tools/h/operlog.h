//***********************************************************************
//  ��ୠ� ����権.                                        JV R-Style  *
//  26.3.1994                                                           *
// 4.4.95 KA ��ࠢ��� ���⨯ ViewOpLog                               *
//  VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/operlog.h $
//                   $Revision: 19 $
//                   $Date: 10.07.00 19:28 $
//                   $Author: Lebedev $
//                   $Modtime: 10.07.00 19:27 $
//***********************************************************************

#ifndef __OPERLOG_H
#define __OPERLOG_H
#define _OPERLOGH_

#include "rstypes.h"
#include "apptldef.h"
#include "menu.h"
#include "btrv.h"
#include "listobj.h"

// SHORT FORMAT: elements and attributes of xml-format for clob-records in operlog\fisclog
#define XML_ELEM_HEXVALUE   "v"
#define XML_ELEM_TABLE      "t"
#define XML_ATTR_TABLENAME  "n"
#define XML_ELEM_FIELD      "f"
#define XML_ATTR_FIELDNAME  "n"
#define XML_ELEM_OLDVALUE   "o"
#define XML_ELEM_NEWVALUE   "v"

// LONG FORMAT: elements and attributes of xml-format for clob-records in operlog\fisclog
#define XML_ELEM_HEXVALUE_LONG    "value"
#define XML_ELEM_TABLE_LONG       "table"
#define XML_ATTR_TABLENAME_LONG   "name"
#define XML_ELEM_FIELD_LONG       "field"
#define XML_ATTR_FIELDNAME_LONG   "name"
#define XML_ELEM_OLDVALUE_LONG    "oldVal"
#define XML_ELEM_NEWVALUE_LONG    "newVal"

// ���� xml-�ଠ⮢ � ������ ��ୠ���������� �����
// ����, ������ �ଠ� (value, table, name � �.�.)
#define    XMLLOG_FORMAT1  1  
// ����, ��⪨� �ଠ� (v, t, n � �.�.)
#define    XMLLOG_FORMAT2  2  

enum EXmlLogFieldType
{
   e_XmlLogField_clob = 0,
   e_XmlLogField_XmlType
};


// �����㥬� ����樨
enum OperLog_Operations
   {
    OLinsert  = 2,                 // ���� �����
    OLupdate  = 3,                 // ���������� �����
    OLdelete  = 4,                 // �������� �����

    OLentry   = 1000,              // �室 � �ணࠬ��
    OLexit    = 1001,              // ��室 �� �ணࠬ��

    OLstrproc = 1002,              // ��砫� �믮������ ��楤���
    OLfinproc = 1003,              // ����砭�� �믮������ ��楤���

    OLinstrn  = 1004,              // �࠭����� �����
    OLupdtrn  = 1005,              // �࠭����� ���������
    OLdeltrn  = 1006,              // �࠭����� 㤠�����

    OLentfail = 1007,              // ��㤠�� �室 � �ணࠬ��

    OLusrdef  = 1100               // ��।��塞� ���짮��⥫�� ����樨
   };



/******************************************************************************
   ���� ����� � ���� ���� ��ୠ�� ����権

   OPERLOG, OPERLOG_BUFKEY
******************************************************************************/

#define  sizeFN    13             // ����� ����� 䠩��
#define  sizeAppl  1024*16        // ����� ���ଠ樨 �ਫ������

#include <packpsh1.h>

// ��ୠ� ����権
typedef struct
      {
       db_bdate   bdDateRec;           // ��� �����
       db_btime   btTimeRec;           // �६� �����
       db_int16   iOper;               // ����� �ᯮ���⥫�
       char       networkNumber[4];
       char       physicalNodeAddress[6];
       db_int16   iOperation;          // ������
       db_int16   iNumberDir;          // ����� ��४���
       char       szFileName[sizeFN];  // ��� 䠩��
       db_int16   iApplicationKind;    // ��� �ਫ������
       char       Reserve[12];         // �����
       db_uint32  crc32;               // ����஫쭠� �㬬�
       db_int32   recId;               // �����䨪��� �����
       db_int16   iSizeVarLen;         // ����� ��६����� ���
       db_int16   iSizeVarLenS;        // ����� ���� �� �믮������
       char       szApplication[sizeAppl]; // ���ଠ�� �ਫ������
      } OPERLOG;

typedef union
      {
       db_bdate      bdDateRec;        // ��� �����
       db_baseint32  recId;            // �����䨪��� �����
      } OPERLOG_BUFKEY;

#include <packpop.h>


// -----------------------------------------------------------------------------
// ���� "��ன" ����� OPERLOG_3 (��� ����� sysCompName/sysUserName)

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // ���-�६� ����樨
       db_btime    time;

       db_int16    numDprt;          // 䨫���
       char        progID;           // �����䨪��� �ணࠬ��
       db_int16    userID;           // �����䨪��� ���짮��⥫�
       char        userType;         // ⨯ ���짮��⥫�
       db_int16    opCode;           // ��� ����樨

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // ����஫쭠� �㬬�
       db_int32    recID;            // �����䨪��� �����

       char        tableName[21];    // ��� ⠡���� � ��
       db_int16    objectType;       // ⨯ ��ꥪ�
       char        objectID[41];     // �����䨪��� ��ꥪ�

       char        reserv[46];       // १��

       db_int32    transComRecID;    // Id �室�饣� ᮮ�饭�� �࠭ᯮ�⭮� ���������� (��) (49-52 ����� ����
                                     // t_Reserve � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // ����� �孮�����᪮�� ���짮��⥫�-஡��, �� ����� ���ண� ����饭
                                     // ���짮��⥫�᪨� ����� �ࢥ� �ਫ������, ���ᯥ稢��騩 ����㭨����
                                     // � ���譥� ��⥬��

       db_uint32   varLenXML;        // ����� ��६����� ��� � xml-������� ��ୠ�� (53-56 ����� ���� t_Reserve
                                     // � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // ����� ��६����� ��� �᫨ > 64K (57-60 ����� ���� t_Reserve � �����
                                     // ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // ����� ��६����� ��� < 64K. �᫨ ࠢ�� 1, � ࠧ��� > 64K � varLenBig
      } OPERLOG_3;

typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

      } OPERLOG_3_BUFKEY;
#include <packpop.h>

/******************************************************************************
   ���� ����� � ���� ���� ��ୠ�� ����権

   OPERLOG_4, OPERLOG_4_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // ���-�६� ����樨
       db_btime    time;

       db_int16    numDprt;          // 䨫���
       char        progID;           // �����䨪��� �ணࠬ��
       db_int16    userID;           // �����䨪��� ���짮��⥫�
       char        userType;         // ⨯ ���짮��⥫�
       db_int16    opCode;           // ��� ����樨

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // ����஫쭠� �㬬�
       db_int32    recID;            // �����䨪��� �����

       char        tableName[21];    // ��� ⠡���� � ��
       db_int16    objectType;       // ⨯ ��ꥪ�
       char        objectID[41];     // �����䨪��� ��ꥪ�

       char        reserv[46];       // १��

       db_int32    transComRecID;    // Id �室�饣� ᮮ�饭�� �࠭ᯮ�⭮� ���������� (��) (49-52 ����� ����
                                     // t_Reserve � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // ����� �孮�����᪮�� ���짮��⥫�-஡��, �� ����� ���ண� ����饭
                                     // ���짮��⥫�᪨� ����� �ࢥ� �ਫ������, ���ᯥ稢��騩 ����㭨����
                                     // � ���譥� ��⥬��

       db_uint32   varLenXML;        // ����� ��६����� ��� � xml-������� ��ୠ�� (53-56 ����� ���� t_Reserve
                                     // � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // ����� ��६����� ��� �᫨ > 64K (57-60 ����� ���� t_Reserve � �����
                                     // ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // ����� ��६����� ��� < 64K. �᫨ ࠢ�� 1, � ࠧ��� > 64K � varLenBig

       char        sysCompName[dNAME_LEN_COMP];  // ��� ��������
       char        sysUserName[dNAME_LEN_USER];  // ��� ���짮��⥫�
      } OPERLOG_4;


typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

       struct
            {
             db_baseint32  recID;
            } recid;

      } OPERLOG_4_BUFKEY;

#include <packpop.h>


/******************************************************************************
   ���� ����� � ���� ���� ��ୠ�� ����権

   OPERLOG_5, OPERLOG_5_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // ���-�६� ����樨
       db_btime    time;

       db_int16    numDprt;          // 䨫���
       char        progID;           // �����䨪��� �ணࠬ��
       db_int16    userID;           // �����䨪��� ���짮��⥫�
       char        userType;         // ⨯ ���짮��⥫�
       db_int16    opCode;           // ��� ����樨

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // ����஫쭠� �㬬�
       db_int32    recID;            // �����䨪��� �����

       char        tableName[21];    // ��� ⠡���� � ��
       db_int16    objectType;       // ⨯ ��ꥪ�
       char        objectID[41];     // �����䨪��� ��ꥪ�

       char        reserv[46];       // १��

       db_int32    transComRecID;    // Id �室�饣� ᮮ�饭�� �࠭ᯮ�⭮� ���������� (��) (49-52 ����� ����
                                     // t_Reserve � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // ����� �孮�����᪮�� ���짮��⥫�-஡��, �� ����� ���ண� ����饭
                                     // ���짮��⥫�᪨� ����� �ࢥ� �ਫ������, ���ᯥ稢��騩 ����㭨����
                                     // � ���譥� ��⥬��

       db_uint32   varLenXML;        // ����� ��६����� ��� � xml-������� ��ୠ�� (53-56 ����� ���� t_Reserve
                                     // � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // ����� ��६����� ��� �᫨ > 64K (57-60 ����� ���� t_Reserve � �����
                                     // ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // ����� ��६����� ��� < 64K. �᫨ ࠢ�� 1, � ࠧ��� > 64K � varLenBig

       char        sysCompName[dNAME_LEN_COMP];  // ��� ��������
       char        sysUserName[dNAME_LEN_USER];  // ��� ���짮��⥫�

       db_int32    processID;        // �����䨪��� ����� (��� ��ᨨ - �� ��� ���)
      } OPERLOG_5;


typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint32  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

       struct
            {
             db_baseint32  recID;
            } recid;

      } OPERLOG_5_BUFKEY;

#include <packpop.h>

/******************************************************************************
   ���� ����� � ���� ���� ��ୠ�� ����権

   OPERLOG_6, OPERLOG_6_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       db_bdate    date;             // ���-�६� ����樨
       db_btime    time;

       db_int16    numDprt;          // 䨫���
       char        progID;           // �����䨪��� �ணࠬ��
       db_int16    userID;           // �����䨪��� ���짮��⥫�
       char        userType;         // ⨯ ���짮��⥫�
       db_int16    opCode;           // ��� ����樨

       db_uint32   netaddr;          // networkNumber
       char        saddr[6];         // physicalNodeAddress

       db_uint32   crc32;            // ����஫쭠� �㬬�
       db_int64    recID;            // �����䨪��� �����
       
       char        tableName[21];    // ��� ⠡���� � ��
       db_int16    objectType;       // ⨯ ��ꥪ�
       char        objectID[41];     // �����䨪��� ��ꥪ�

       char        reserv[46];       // १��

       db_int32    transComRecID;    // Id �室�饣� ᮮ�饭�� �࠭ᯮ�⭮� ���������� (��) (49-52 ����� ����
                                     // t_Reserve � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt)) 

       db_int16    robot;            // ����� �孮�����᪮�� ���짮��⥫�-஡��, �� ����� ���ண� ����饭
                                     // ���짮��⥫�᪨� ����� �ࢥ� �ਫ������, ���ᯥ稢��騩 ����㭨����
                                     // � ���譥� ��⥬��

       db_uint32   varLenXML;        // ����� ��६����� ��� � xml-������� ��ୠ�� (53-56 ����� ���� t_Reserve
                                     // � ����� ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint32   varLenBig;        // ����� ��६����� ��� �᫨ > 64K (57-60 ����� ���� t_Reserve � �����
                                     // ⠡���� ��ୠ�� (dfisclog_dbt, doperlog_dbt))
       db_uint16   varLen;           // ����� ��६����� ��� < 64K. �᫨ ࠢ�� 1, � ࠧ��� > 64K � varLenBig

       char        sysCompName[dNAME_LEN_COMP];  // ��� ��������
       char        sysUserName[dNAME_LEN_USER];  // ��� ���짮��⥫�

       db_int32    processID;        // �����䨪��� ����� (��� ��ᨨ - �� ��� ���)
      } OPERLOG_6;


typedef union
      {
       struct
            {
             db_bdate  date;
             db_btime  time;
            } stamp;

       struct
            {
             db_baseint16  numDprt;
             db_baseint64  recID;
            } id;

       struct
            {
             db_baseint16  numDprt;
             db_bdate      date;
             db_btime      time;
            } dprt;

       struct
            {
             char      progID;
             db_bdate  date;
             db_btime  time;
            } prog;

       struct
            {
             db_baseint16  userID;
             db_bdate      date;
             db_btime      time;
            } user;

       struct
            {
             char      tableName[21];
             db_bdate  date;
             db_btime  time;
            } table;

       struct
            {
             db_baseint16  objectType;
             db_bdate      date;
             db_btime      time;
            } type;

       struct
            {
             char      objectID[41];
             db_bdate  date;
             db_btime  time;
            } obj;

       struct
            {
             db_baseint32  transComRecID;               
            } transcom;

       struct
            {
             db_baseint64  recID;
            } recid;

      } OPERLOG_6_BUFKEY;

#include <packpop.h>


// -----------------------------------------------------------------------------
enum                             // ����� ���祩
   {
    OPERLOG_DATE = 0,
    OPERLOG_ID,
    OPERLOG_DPRT,
    OPERLOG_PROG,
    OPERLOG_USER,
    OPERLOG_TABLE,
    OPERLOG_TYPE,
    OPERLOG_OBJ,
    OPERLOG_TRANSCOMRECID,
    OPERLOG_RECID
   };



/******************************************************************************
   ���� ����� � ���� ���� �ᯮ����⥫쭮�� 䠩�� ��ୠ�� ����権

   OPLOGSUP, OPLOGSUP_BUFKEY
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       char       fileName[26];     // 䠩� ��ୠ��
       db_int16   numDprt;          // 䨫���

       db_uint32  crc32;            // ����஫쭠� �㬬�
       db_int32   recID;            // �����䨪��� �����

       db_uint32  xml_crc32;        // ����஫쭠� �㬬� ��� xml-����ᥩ

       char       reserv[56];       // १��
      } OPLOGSUP;

typedef union
      {
       struct
            {
             char          fileName[26];
             db_baseint16  numDprt;
            } dprt;
      } OPLOGSUP_BUFKEY;

#include <packpop.h>

/******************************************************************************
   ���� ����� � ���� ���� �ᯮ����⥫쭮�� 䠩�� ��ୠ�� ����権

   OPLOGSUP_6, OPLOGSUP_BUFKEY_6
******************************************************************************/

#include <packpsh1.h>

typedef struct
      {
       char       fileName[26];     // 䠩� ��ୠ��
       db_int16   numDprt;          // 䨫���

       db_uint32  crc32;            // ����஫쭠� �㬬�
       db_int64   recID;            // �����䨪��� �����

       db_uint32  xml_crc32;        // ����஫쭠� �㬬� ��� xml-����ᥩ

       char       reserv[56];       // १��
      } OPLOGSUP_6;

typedef union
      {
       struct
            {
             char          fileName[26];
             db_baseint16  numDprt;
            } dprt;
      } OPLOGSUP_BUFKEY_6;

#include <packpop.h>

enum                             // ����� ���祩
   {
    OPLOGSUP_DPRT = 0,
   };



#ifdef __cplusplus
 extern "C" {
#endif

_APPTLDATA extern BTRVFILE  *FileOpLog;     // ���� ��ୠ�� ����権


#define  SKF_OpLog(KeyNum, beg, end)               SKF_OperLog(FileOpLog, KeyNum, beg, end)

#define  iOpenOpLog(iOpenMode, NameFile)           ((FileOpLog = Open_OperLog(iOpenMode, NameFile, NULL)), bfstat)
#define  iOpenOpLogEx(iOpenMode, NameFile, owner)  ((FileOpLog = Open_OperLog(iOpenMode, NameFile, owner)), bfstat)

#define  iCrOperLog(NameFile)                      Create_OperLog(NameFile)

#define  iWriteOpLog(iOperation, iOper, iApplicationKind, iNumberDir, bfile, saveb, iSizeVarLenS) \
         Write_OperLog(FileOpLog, iOperation, iOper, iApplicationKind, iNumberDir, bfile, saveb, iSizeVarLenS)

#define  ViewOpLog(NameDict, ViewAppl)     View_OperLog(FileOpLog, NameDict, ViewAppl)

//***********************************************************************

// ������� ᯨ᪠ ����権
typedef struct
      {
       LOBJ  node;
       int   id;                     // �����䨪��� ����樨
       int   categ;                  // ��⥣��� ����樨
       char  name[21];               // ������������
      } OpLogOper;

// ��⥣�ਨ ��ୠ�����㥬�� ����権
enum
   {
    OLinsrec = 1,                // ���� �����
    OLdelrec = 2,                // �������� �����
    OLchgrec = 3,                // ���������� �����
    OLproc   = 4                 // ��楤��
   };

/*
 * InitOpLogOper
 *    ���樠������ ᯨ᪠ ����権.
 *    �������� ᯨ᪠ � ���ᥭ�� � ���� �⠭������ ����権.
 *
 * AddOpLogOper
 *    �������� ������ � ᯨ᮪. id ������ ���� 㭨����� � ᯨ᪥.
 *
 * FindOpLogOper
 *    ���� ������ � ᯨ᪥. �����頥� 㪠��⥫� �� �������� ����� ᯨ᪠.
 *    ��� NULL, �᫨ ������ �� �������.
 */

_APPTLFUNC int        InitOpLogOper(void);
_APPTLFUNC int        AddOpLogOper (int id, int categ, const char *name);
_APPTLFUNC OpLogOper *FindOpLogOper(int id);

//***********************************************************************

/*
 *    ������⢮ �㭪権 ��� �����প� ��ୠ�� ����権, � ������������
 *          �����६������ �ᯮ�짮����� ��᪮�쪨� ��ୠ���.
 *
 *    Create_OperLog
 *    Open_OperLog
 *    SKF_OperLog
 *    Write_OperLog
 *    View_OperLog
 *
 * �㭪�� Create_OperLog ��祬 �� �⫨砥��� �� ��ன iCrOperLog.
 *
 * ����� ���� �㭪権 iOpenOpLog � iOpenOpLogEx ᤥ���� ���� - Open_OperLog.
 * ��� ����� �� ��ࠬ��� (��⨩ �� ��� ��������) ��� � iOpenOpLogEx,
 * ࠧ��� ����� ���� � ⮬, �� ��� �����頥� �� ��� �訡��, � 㪠��⥫� ��
 * ������ 䠩� ��� NULL, �᫨ 䠩� �� �����.
 *
 * ��⠫�� �㭪樨 ��������� ���� �㭪�� �� �᪫�祭��� ⮣�, ��
 * ���� ��ࠬ��� �� 㪠��⥫� �� ������ 䠩� ��ୠ��.
 * ���짮��⥫� ������ (��� � �०��) ᠬ ����������� � �����⨨ ����⮣�
 * 䠩��, �맢�� bfClose.
 *
 */
_APPTLFUNC int        Create_OperLog(char *NameFile);
_APPTLFUNC BTRVFILE *Open_OperLog   (int OpenMode, const char *NameFile, const char *Owner);
_APPTLFUNC void       SKF_OperLog   (BTRVFILE *, int KeyNum, db_bdate* Beg, db_bdate* End);

_APPTLFUNC int        Write_OperLog (BTRVFILE *, int Operation, int oper, int ApplictionKind, int NumberDir,
                                     BTRVFILE *bfile, void *saveb, int size);

_APPTLFUNC int        View_OperLog  (BTRVFILE *, const char *NameDict, panel_keyproc_t proc);


// �㭪�� ViewOpLogItem - ��ᬮ�� ����� �� ��ୠ�� ����権
_APPTLFUNC void       ViewOpLogItem (OPERLOG *ol, BTRVFILE **ProjFiles);


//***********************************************************************

// ������� ᯨ᪠
typedef struct
      {
       LOBJ  node;
       char *FileName;              // ��� 䠩�� - 㭨����� ���� � ᯨ᪥
       int   present_id;            // �ਧ��� ������ � 䠩�� ���� recID
      } OpenOperLog;

void            SetFlagID_OpenOperLog(const char *FileName, int present_id);
_APPTLFUNC int  GetFlagID_OpenOperLog(const char *FileName);

/*
 * �஢���� 楫��⭮��� ����ᥩ � ��ୠ�� ����権.
 */
typedef int (* CheckCRCindic_t)(void*);

_APPTLFUNC int CheckCRC_OperLog(BTRVFILE *, CheckCRCindic_t indic, void *parm);

#ifdef __cplusplus
 } ;
#endif

#endif // __OPERLOG_H

//************************** ����� 䠩�� ********************************

/* <-- EOF --> */