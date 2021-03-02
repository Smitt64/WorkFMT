/***************************************************************************
*  Source File  : PCX.H
*  Group        : Header
*  Hint         : RS Bank v3.2 Sign Viewer
*  Compiler     : Borland C++ v 3.1
*  Memory Model : Large
*  NB           : None
*  History      : 26 nov 1993 created by Mike Denisov
*                 v 1.0
*  Respons.     : Mike Denisov
*  Copyright    : JV R Style (C) 1993.
****************************************************************************/

#ifndef _PCX_H_
#define _PCX_H_

enum
   {
    PIC_VIEW,    // View Sign
    PIC_IMPORT,  // Import Sign
    PIC_DELETE   // Delete Sign
   };

// Show,Import or Delete image
int Sign(char *szCode, int iCmd, char *szName, char *szDescr);

// szCode - Client's code.
// iCmd   - PIC_VIEW or PIC_IMPORT or PIC_DELETE.
// szName - sign file database name.
//          >if 'SignFile' was opened by OpenSignFile you can set szName
//           to NULL.
//          >if  SignFile = NULL (file not opened) you must assign szName.
//           SignFile will be temporary opened to perform the operation.
// szDescr- Description String.
//          You can set it to NULL.

int SignInt(int iCode, int iCmd, char *szName, char *szDescr);

//Open & Close Sign File
int OpenSignFile(char *szFile, int iSay);

// Set SignFile . iSay = ON,OFF ( Output error message )
int CloseSignFile(void);

// Sign File
extern BTRVFILE  *SignFile;

#endif // _PCX_H_

/* <-- EOF --> */
