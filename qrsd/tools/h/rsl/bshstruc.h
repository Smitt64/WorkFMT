/*******************************************************************************
 FILE         :   BSHCTRUC.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   ������ ᫮���� ���� ������

 PROGRAMMED BY:

 CREATION DATE:

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsl/bshstruc.h $
                  $Revision: 13 $
                  $Date: 26.06.00 19:31 $
                  $Author: Lebedev $
                  $Modtime: 26.06.00 19:24 $
*******************************************************************************/

#ifndef __BSHSTRUC_H
#define __BSHSTRUC_H

#ifndef __NWBTRV_H
#include "nwbtrv.h"
#endif

#ifndef __RSTYPES_H
#include "rstypes.h"
#endif

#include "packpsh1.h"

// -----------------------------------------------------------------------------
/* OLD_STRUCT DEF-FILE */
// ���ᠭ�� ����� �஥��
typedef union
      {
       // ���ᠭ�� 䠩�� Btrieve
       struct
            {
             db_baseint32   Code_File;      // ��� 䠩��
             unsigned char  Group;          // �ਧ��� ��㯯�
             char           Name[13];       // ��������
             char           OWN[9];         // ��������
             db_baseint16   TypeOWN;        // ��� ��������
             char           Contents[33];   // ����ঠ���
             FileSpecs      FS;             // ����䨪��� 䠩��
             db_baseint16   maxvarlen;      // ���ᨬ��쭠� ����� ��६����� ���.
             char           UseBLOB;        // �ᯮ�짮����� VarLen as BLOB
             char           isTmp;          // GLOBAL TEMPORARY TABLE  for Oracle version.
            } bfile;

       // ���ᠭ�� ���� 䠩�� Btrieve
       struct
            {
             db_baseint32   Code_Field;     // ��� ����
             unsigned char  Group;          // �ਧ��� ��㯯�
             db_baseint32   Code_File;      // ��� 䠩��
             db_baseint16   Number;         // ����� ����
             char           Name[24];       // ��������
             unsigned char  FVt;            // ��� ���祭�� ����
             db_baseint16   Field_Point;    // ������ ��᫥ �窨, ����. ����� ��ப�
             db_baseint16   l;              // ����� ��ப� �뢮��
             db_baseint16   fldlen;         // ����� ���� ����
             char           Contents[37];   // ����ঠ���
             char           ViewS[2];       // ����� � �஫����
             char           ViewP[2];       // ����� � ������

             unsigned char  ViewE;          // ����� � �ᯮ��
             unsigned char  decPoint;       // ������⢮ ������ ��᫥ �窨 ��� NUMERIC ⨯� ����
             unsigned char  ViewI;          // ����� � ������
             unsigned char  reserv;
            } bfield;

       // ���ᠭ�� ���� 䠩�� Btrieve
       struct
            {
             db_baseint32   Code_Key;       // ��� ����
             unsigned char  Group;          // �ਧ��� ��㯯�
             db_baseint32   Code_File;      // ��� 䠩��
             db_baseint16   Number;         // ����� ����
             db_baseint32   Code_Field;     // ��� ����
             db_baseint16   offset;         // ���饭�� �⭮�⥫쭮 ��砫� ����
             char           Contents[22];   // ����ঠ���
             KeySpecs       KS;             // ����䨪��� ����
             unsigned char  pKey;           // The number of Primary Key
            } bkey;

       struct
            {
             db_baseint32   Code_Vers;      // ��� �����
             unsigned char  Group;          // �ਧ��� ��㯯�
             char           Name[13];       // ��������
             db_baseint32   Version;        // ����� ���ᨨ
            } version;
      } BUFREC_SIZE87;

// -----------------------------------------------------------------------------
// ���� ���� 䠩�� �஥��
typedef union
      {
       db_baseint32         Code_File;      // ��� 䠩��

       struct
            {
             unsigned char  Group;          // �ਧ��� ��㯯�
             char           Name[13];       // ��������
            } GN;

       struct
            {
             db_baseint32   Code_File;      // ��� 䠩��
             unsigned char  Group;          // �ਧ��� ��㯯�
             db_baseint16   Number;         // ����� ����
            } GFN;
      } BUFKEY_SIZE87;

// -----------------------------------------------------------------------------
#ifndef _MSC_VER
    #ifndef __cplusplus
        #if (sizeof(BUFREC_SIZE87) != 87)
            #error "Incorrect sizeof(BUFREC_SIZE87) != 87"
        #endif

        #if (sizeof(BUFKEY_SIZE87) != 14)
            #error "Incorrect sizeof(BUFKEY_SIZE87) != 14"
        #endif
    #endif
#endif  // _MSC_VER

// -----------------------------------------------------------------------------
// ���� ����� �஥��
enum BRECKEY
   {
    KEYAUTO = 0,                   // 0 - Code_File
    KEYGN,                         // 1 - Group, Name
    KEYGFN                         // 2 - Code_File, Group, Number
   };

// -----------------------------------------------------------------------------
// ��㯯� ����ᥩ �஥��
enum BSHBGROUP
   {
    BFE = 'f',                     // ���ᠭ�� 䠩��
    BFD = 'd',                     // ���ᠭ�� ����
    BKY = 'k',                     // ���ᠭ�� ����
    BVR = '\0'                     // ���ᠭ�� ���ᨨ
   };


// -----------------------------------------------------------------------------
#define  RSL_DEF_VERSION  "VERSION"

// -----------------------------------------------------------------------------
/* NEW_STRUCT DEF-FILE */
//RECORD DESCRIPTIONS
// ���ᠭ�� ����� �஥��
typedef struct
      {
       //COMMON & KEY
       struct BUFREC_common
            {
             long           Code;               // ��� ����� (䠩��, ����, ���� ��� ���ᨨ)
             unsigned char  Group;              // �ਧ��� ��㯯�
             short int      Number;             // ����� ���� ��� ����
             char           Name[24];           // �������� 䠩�� ��� ����
             long           Code_File;          // ��� 䠩�� ��� ���� ��� ����
             char           Contents[40];       // ����ঠ���
            } common;

       //VERSION
       struct
            {
             long           Version;            // ����� ���ᨨ
            } version;

       //BFILE
       struct
            {
             char           OWN[9];             // ��������
             short int      TypeOWN;            // ��� ��������
             FileSpecs      FS;                 // ����䨪��� 䠩��
             short int      maxvarlen;          // ���ᨬ��쭠� ����� ��६����� ���.
             char           UseBLOB;            // �ᯮ�짮����� VarLen as BLOB
            } bfile;

       //BFIELD
       struct
            {
             unsigned char  FVt;                // ��� ���祭�� ����
             short int      Field_Point;        // ������ ��᫥ �窨, ����. ����� ��ப�
             short int      l;                  // ����� ��ப� �뢮��
             short int      fldlen;             // ����� ���� ����
             char           ViewS[2];           // ����� � �஫����
             char           ViewP[2];           // ����� � ������
             short int      ViewE;              // ����� � �ᯮ��
             short int      ViewI;              // ����� � ������
            } bfield;

       //BKEY
       struct
            {
             long           Code_Field;         // ��� ����
             short int      offset;             // ���饭�� �⭮�⥫쭮 ��砫� ����
             KeySpecs       KS;                 // ����䨪��� ����
            } bkey;
      } BUFREC_NONOVERLAPPED_RECORD;

// -----------------------------------------------------------------------------
// ���ᠭ�� ����� �஥��
typedef union BUFREC_NONOVERLAPPED
      {
       //VERSION
       struct
            {
             long           Code_Vers;          // ��� �����
             unsigned char  Group;              // �ਧ��� ��㯯�
             short int      Number;             // ����� ���� ��� ����
             char           Name[24];           // ��������
             long           reserv2;
             char           Contents[40];       // ����ঠ���
             long           Version;            // ����� ���ᨨ
            } version;

       //FILE DESCRIPTIONS
       // ���ᠭ�� 䠩�� Btrieve
       struct
            {
             long           Code_File;          // ��� 䠩��
             unsigned char  Group;              // �ਧ��� ��㯯�
             short int      reserv1;
             char           Name[24];           // ��������
             long           reserv2;
             char           Contents[40];       // ����ঠ���

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];

             char           OWN[9];             // ��������
             short int      TypeOWN;            // ��� ��������
             FileSpecs      FS;                 // ����䨪��� 䠩��
             short int      maxvarlen;          // ���ᨬ��쭠� ����� ��६����� ���.
             char           UseBLOB;            // �ᯮ�짮����� VarLen as BLOB
            } bfile;

       //FIELDS DESCRIPTIONS
       // ���ᠭ�� ���� 䠩�� Btrieve
       struct
            {
             long           Code_Field;         // ��� ����
             unsigned char  Group;              // �ਧ��� ��㯯�
             short int      Number;             // ����� ���� ��� ����
             char           Name[24];           // ��������
             long           Code_File;          // ��� 䠩�� ��� ���� ��� ����
             char           Contents[40];       // ����ঠ���

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];
             char           bfile  [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfile)];

             unsigned char  FVt;                // ��� ���祭�� ����
             short int      Field_Point;        // ������ ��᫥ �窨, ����. ����� ��ப�
             short int      l;                  // ����� ��ப� �뢮��
             short int      fldlen;             // ����� ���� ����
             char           ViewS[2];           // ����� � �஫����
             char           ViewP[2];           // ����� � ������
             short int      ViewE;              // ����� � �ᯮ��
             short int      ViewI;              // ����� � ������
            } bfield;

       //KEYS DESCRIPTIONS
       // ���ᠭ�� ���� 䠩�� Btrieve
       struct
            {
             long           Code_Key;           // ��� ����
             unsigned char  Group;              // �ਧ��� ��㯯�
             short int      Number;             // ����� ���� ��� ����
             char           reserv1[24];
             long           Code_File;          // ��� 䠩�� ��� ���� ��� ����
             char           Contents[40];       // ����ঠ���

             char           version[sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->version)];
             char           bfile  [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfile)];

             unsigned char  pKey;               // The number of Primary Key
             char           bfield [sizeof(((BUFREC_NONOVERLAPPED_RECORD *)NULL)->bfield) - 1];

             long           Code_Field;         // ��� ����
             short int      offset;             // ���饭�� �⭮�⥫쭮 ��砫� ����
             KeySpecs       KS;                 // ����䨪��� ����
            } bkey;
      } BUFREC_NONOVERLAPPED;

// -----------------------------------------------------------------------------
#ifndef _MSC_VER
    #ifndef __cplusplus
        #if sizeof(BUFREC_NONOVERLAPPED) != sizeof(BUFREC_NONOVERLAPPED_RECORD)
            #error "Invalid size 'BUFREC_NONOVERLAPPED'"
        #endif
    #endif
#endif // _MSC_VER

// -----------------------------------------------------------------------------
// ���� ���� 䠩�� �஥��
typedef union
      {
       long                 Code_File;      // ��� 䠩��

       struct
            {
             unsigned char  Group;          // �ਧ��� ��㯯�
             char           Name[24];       // ��������
            } GN;

       struct
            {
             long           Code_File;      // ��� 䠩��
             unsigned char  Group;          // �ਧ��� ��㯯�
             short int      Number;         // ����� ����
            } GFN;
      } BUFKEY_NONOVERLAPPED;

#include "packpop.h"

// -----------------------------------------------------------------------------
/* SET DEFAULT STRUCT */
typedef BUFREC_SIZE87  BUFREC;
typedef BUFKEY_SIZE87  BUFKEY;

#endif // __BSHSTRUC_H

/* <-- EOF --> */