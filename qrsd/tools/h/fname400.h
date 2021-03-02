/*******************************************************************************
 FILE         :   FNAME400.H

 COPYRIGHT    :   R-Style Software Lab, 1999

 DESCRIPTION  :   AS/400 filenames parsing

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   14 Jan 1999

 VERSION INFO :   $Archive: /RSTool400/Runtime/include/fname400.h $
                  $Revision: 2 $
                  $Date: 23.04.99 21:52 $
                  $Author: Markovitch $
                  $Modtime: 9.04.99 15:40 $
*******************************************************************************/

#ifndef __FNAME400_H
#define __FNAME400_H

/*******************************************************************************
                     enum QSysFNameFlags

 AS/400 QSYS.LIB filesystem name flags
*******************************************************************************/
enum QSysFNameFlags
   {
    QSYSFN_QSYSLIB =  0x0001,  // The name is QSYS.LIB filesystem name (i.e. correct name
                               // with /QSYS.LIB at the beginning)

    QSYSFN_LIB     =  0x0002,  // The name has a library part (apart from /QSYS.LIB, f.i. /QSYS.LIB/MY.LIB)

    QSYSFN_OBJECT  =  0x0004,  // The name has an object part
                               // (f.i., /QSYS.LIB/MY.LIB/MYFILE.FILE or /QSYS.LIB/MYFILE.FILE)

    QSYSFN_MEMBER  =  0x0008   // The name has a member part (f.i., /QSYS.LIB/MY.LIB/MYFILE.FILE/MYMBR.MBR)
   };

/*******************************************************************************
                     enum QSysFSplitFlags

 qfnsplit option flags
*******************************************************************************/
enum QSysFSplitFlags
   {
    QSPLIT_ANYOBJLEN  =  0x0001,  // The name of object can be any length (more than 10 chars)
    QSPLIT_ANYTYPELEN =  0x0002,  // The type of object can be any length (more than 8 chars)
    QSPLIT_ANYOBJTYP  =  0x0004   // The type of object can consist of any characters besides ./\\t*
   };

/*******************************************************************************
                     enum QSysFSplitnFlags

 qfnsplitn option flags
*******************************************************************************/
enum QSysFSplitnFlags
   {
    QSPLITN_SPECLIB   =  0x0001   // The name of library can be special name (*CURLIB *LIBL)
   } ;

/*
   Buffer sizes (including final zero) for various path components
*/
#define  _QSYS_MAXNAME     11
#define  _QSYS_MAXLIB     _QSYS_MAXNAME
#define  _QSYS_MAXOBJ     _QSYS_MAXNAME
#define  _QSYS_MAXMBR     _QSYS_MAXNAME
#define  _QSYS_MAXTYPE      9
#define  _QSYS_MAXPATH     60
#define  _QSYS_MAXNATIVE   34
#define  _QSYS_MAXANY     256


#ifdef __cplusplus
extern "C" {
#endif

/*
   Split QSYS filesystem path in IFS form ("/QSYS.LIB/MYFILE.FILE ...") into its components.
   Parameters:
      fname    -  source path in correct IFS form (such as "/QSYS.LIB/MYFILE.FILE", etc.)
                  Can be NULL or empty string
      libname  -  pointer to buffer for library name. Can be NULL
      objname  -  pointer to buffer for object name. Can be NULL
      objtype  -  pointer to buffer for object type. Can be NULL
      member   -  pointer to buffer for member name. Can be NULL

   Result:
      If fname is a correct path in QSYS.LIB filesystem, returns value with
      appropriate bits set and fills corresponding (not-NULL) result buffers.
      Bit flags are described in QSysFNameFlags enum.
      If fname is not a correct QSYS.LIB path, returns NULL and leaves all buffers untouched.
*/
unsigned qfnsplit(const char *fname, char *libname, char *objname, char *objtype, char *member, unsigned flags);

/*
   Merge QSYS filesystem path in IFS form from its components.
   Parameters:
      buf      -  buffer for resulting path. Can be NULL
      libname  -  pointer to library name. Can be NULL
      objname  -  pointer to object name. Can be NULL
      objtype  -  pointer to object type. Can be NULL
      member   -  pointer to member name. Can be NULL

   Result:
      if buf != NULL and all components are correct creates path in buf and returns
      pointer to buf.
      Else returns NULL.
*/
char *qfnmerge(char *buf, const char *libname, const char *objname, const char *objtype, const char *member);

/*
   Split QSYS filesystem path in native form ("LIBRARY/OBJECT(MEMBER)") into its components.
   Parameters:
      fname    -  source path in native form. Can be NULL or empty string
      libname  -  pointer to buffer for library name. Can be NULL
      objname  -  pointer to buffer for object name. Can be NULL
      member   -  pointer to buffer for member name. Can be NULL

   Result:
      If fname is a correct native path, returns value with
      appropriate bits set and fills corresponding (not-NULL) result buffers.
      Bit flags are described in QSysFNameFlags enum.
      If fname is not a correct path, returns NULL and leaves all buffers untouched.
*/
unsigned qfnsplitn(const char *fname, char *libname, char *objname, char *member, unsigned flags);

/*
   Merge QSYS filesystem path in native form from its components.
   Parameters:
      buf      -  buffer for resulting path. Can be NULL
      libname  -  pointer to library name. Can be NULL
      objname  -  pointer to object name. Can be NULL
      member   -  pointer to member name. Can be NULL

   Result:
      if buf != NULL and all components are correct creates path in buf and returns
      pointer to buf.
      Else returns NULL.
*/
char *qfnmergen(char *buf, const char *libname, const char *objname, const char *member);

/*
   Check QSYS filesystem path to determine if it exists, and
   whether it can be read, written to, or executed.
*/
int qaccess(const char *path, const char *type, int mode);

#ifdef __cplusplus
}
#endif

#endif // __FNAME400_H

/* <-- EOF --> */