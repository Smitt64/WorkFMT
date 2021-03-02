/* pcxLib.H                                                                 */
/* Copyright (c) Genus Microprogramming, Inc. 1988-89  All Rights Reserved. */

/****************************************************************************

  Function declarations for the PCX Programmer's Toolkit, for C.


  Microsoft C version 5.1                 Programmer: Chris Howard  3/02/88

*****************************************************************************/

#ifndef _PCXLIB_H_
#define _PCXLIB_H_

/* PCX Version number */
#define  pcxhard       5                      /* Version = 3.0 w/palette     */

/* PCX Display modes */
#define  pcxTEXT       0                      /* Text mode                   */
#define  pcxGRAPHICS   1                      /* Graphics mode               */

/* PCX Display types */
#define  pcxMINDISP    0
#define  pcxCGA_4      0                      /* CGA Mode  4H (320x200x4)    */
#define  pcxCGA_6      1                      /* CGA Mode  6H (640x200x2)    */
#define  pcxEGA_D      2                      /* EGA Mode  DH (320x200x16)   */
#define  pcxEGA_E      3                      /* EGA Mode  EH (640x200x16)   */
#define  pcxEGA_F      4                      /* EGA Mode  FH (640x350x2)    */
#define  pcxEGA_10     5                      /* EGA Mode 10H (640x350x16)   */
#define  pcxVGA_11     6                      /* VGA Mode 11H (640x480x2)    */
#define  pcxVGA_12     7                      /* VGA Mode 12H (640x480x16)   */
#define  pcxVGA_13     8                      /* VGA Mode 13H (320x200x256)  */
#define  pcxHERC       9                      /* Hercules     (720x348x2)    */

/* New extended VGA modes */
#define  pcxXGA_29     10                     /* Tseng    29H (800x600x16)   */
#define  pcxXGA_2D     11                     /* Tseng    2DH (640x350x256)  */
#define  pcxXGA_2E     12                     /* Tseng    2EH (640x480x256)  */
#define  pcxXGA_30     13                     /* Tseng    30H (800x600x256)  */
#define  pcxXGA_58     14                     /* Paradise 58H (800x600x16)   */
#define  pcxXGA_59     15                     /* Paradise 59H (800x600x2)    */
#define  pcxXGA_5E     16                     /* Paradise 5EH (640x400x256)  */
#define  pcxXGA_5F     17                     /* Paradise 5FH (640x480x256)  */
#define  pcxXGA_62     18                     /* Video 7  62H (800x600x16)   */
#define  pcxXGA_66     19                     /* Video 7  66H (640x400x256)  */
#define  pcxXGA_67     20                     /* Video 7  67H (640x480x256)  */
#define  pcxXGA_69     21                     /* Video 7  69H (800x600x256)  */
#define  pcxMAXDISP    21

/* Printer Ports */
#define  pcxLPT1       0                      /* Printer port 1              */
#define  pcxLPT2       1                      /* Printer port 2              */
#define  pcxLPT3       2                      /* Printer port 3              */

/* Printer Types */
#define  pcxMINPRINT   0                      /* Minimum printer type        */
#define  pcxLASER      0                      /* HP LaserJet                 */
#define  pcxDOTMAT     1                      /* Dot Matrix                  */
#define  pcxMAXPRINT   1                      /* Maximum printer type        */

/* Printer Modes */
#define  pcxDRAFT      0                      /* Draft mode                  */
#define  pcxQUALITY    1                      /* Quality                     */
#define  pcxFAX        2                      /* Fax                         */

/* Printer Scales */
#define  pcxSCALE100   0                      /* 100%                        */
#define  pcxSCALE75    1                      /*  75%                        */
#define  pcxSCALE50    2                      /*  50%                        */
#define  pcxSCALE25    3                      /*  25%                        */
#define  pcxSCALE200   4                      /* 200%                        */

/* Printer Flags */
#define  pcxNORESET    0                      /* No reset before printing    */
#define  pcxRESET      1                      /* Reset before printing       */
#define  pcxNOEJECT    0                      /* No eject after printing     */
#define  pcxEJECT      1                      /* Eject page after printing   */

/* Searches */
#define  pcxFIRST      0                      /* Find first matching entry   */
#define  pcxNEXT       1                      /* Find next matching entry    */

/* Logical Operations */
#define  pcxSET        0                      /* SET pixel value directly    */
#define  pcxAND        1                      /* AND pixel value with data   */
#define  pcxOR         2                      /* OR  pixel value with data   */
#define  pcxXOR        3                      /* XOR pixel value with data   */

/* Virtual screens */
#define  pcxEMM        0                      /* Expanded Memory virtual     */
#define  pcxCMM        1                      /* Conventional Memory virtual */

/* Miscellaneous */
#define  pcxTRUE       1                      /* True value                  */
#define  pcxFALSE      0                      /* False value                 */
#define  pcxSYNC       1                      /* PCX library synch value     */

/* Video Info Constants */
#define  viNONE        0                      /* No adapter/display          */
#define  viMDA         1                      /* Monochrome Display Adapter  */
#define  viCGA         2                      /* Color Graphics Adapter      */
#define  viEGA         3                      /* Enhanced Graphics Adapter   */
#define  viMCGA        4                      /* MCGA                        */
#define  viVGA         5                      /* Video Graphics Array        */
#define  viHGC         0x80                   /* Hercules Monochrome         */
#define  viHGCPlus     0x81                   /* Hercules Plus               */
#define  viHGCColor    0x82                   /* Hercules InColor            */

#define  viMDAdisp     1                      /* Monochrome Display          */
#define  viCGAdisp     2                      /* CGA Display                 */
#define  viEGAdisp     3                      /* EGA Display                 */
#define  viVGAmono     4                      /* VGA Monochrome Display      */
#define  viVGAcolor    5                      /* VGA Color Display           */

/* Chip sets */
#define  chipUNKNOWN   0                      /* Unknown chip set            */
#define  chipTSENG     1                      /* Tseng Labs                  */
#define  chipPARA      2                      /* Paradise                    */
#define  chipV7        3                      /* Video 7                     */

/* Hercules Configuration */
#define  pcxHERCdiag   0                      /* Hercules Diagnostic         */
#define  pcxHERChalf   1                      /* Hercules Half               */
#define  pcxHERCfull   3                      /* Hercules Full               */

/* Error Codes */
#define  pcxSUCCESS          0                /* Successful                  */
#define  pcxERR_OPEN        -1                /* Error opening file          */
#define  pcxERR_BUFSMALL    -2                /* Buffer is too small         */
#define  pcxERR_READ        -3                /* Error in reading from file  */
#define  pcxERR_WRITE       -4                /* Error in writing to file    */
#define  pcxERR_NOTPCX      -5                /* Not a valid pcx buffer/file */
#define  pcxERR_BADDISP     -6                /* Display not defined/invalid */
#define  pcxERR_BADMODE     -7                /* Bad display mode            */
#define  pcxERR_BADPAGE     -8                /* Bad page                    */
#define  pcxERR_BADPAL      -9                /* Bad palette                 */
#define  pcxERR_NOPAL       -10               /* No palette available        */
#define  pcxERR_NOPALGRAB   -11               /* Palette is not grabbed      */
#define  pcxERR_NOTPCXLIB   -12               /* Not a PCX library           */
#define  pcxERR_NOSYNC      -13               /* Lost image synch in library */
#define  pcxERR_NOTINLIB    -14               /* Image not found in library  */
#define  pcxERR_CLIPPARM    -15               /* Bad number of Clipper parms */
#define  pcxERR_NOTIMPL     -900              /* Not implemented yet         */
#define  pcxERR_GENERAL     -999              /* General error               */

/* New errors for version 3.5 */
#define  pcxERR_BADPRINT    -16               /* Bad printer type            */
#define  pcxERR_BADPORT     -17               /* Bad printer port            */
#define  pcxERR_PRINTER     -18               /* Pr err (timeout,i/o,paper)  */
#define  pcxERR_NOTBW       -19               /* Not a Black/White image     */
#define  pcxERR_ESCAPE      -20               /* Escape key was pressed      */
#define  pcxERR_NOEMM       -21               /* No EMM driver was found     */
#define  pcxERR_EMMFAIL     -22               /* General EMM driver failure  */
#define  pcxERR_EMMNOMEM    -23               /* No EMM memory available     */
#define  pcxERR_BADMEMTYPE  -24               /* Bad virtual memory type     */
#define  pcxERR_CMMFAIL     -25               /* General CMM alloc/free err  */
#define  pcxERR_CMMNOMEM    -26               /* No CMM memory available     */


/* PCX File Header */
typedef struct pcxheader
      {
       char         manuf;                 /* Always =10 for Paintbrush   */
       char         hard;                  /* Version information         */
       char         encod;                 /* Run-length encoding (=1)    */
       char         bitpx;                 /* Bits per pixel              */
       unsigned     x1;                    /* Picture dimensions (incl)   */
       unsigned     y1;
       unsigned     x2;
       unsigned     y2;
       unsigned     hres;                  /* Display horiz resolution    */
       unsigned     vres;                  /* Display vert  resolution    */
       char         clrma[48];             /* Pallete                     */
       char         vmode;                 /* (ignored)                   */
       char         nplanes;               /* Number of planes (ver 2.5=0)*/
       unsigned     bplin;                 /* Bytes per line              */
       unsigned     palinfo;               /* Palette Info (1=col, 2=gray)*/
       unsigned     shres;                 /* Scanner resolution          */
       unsigned     svres;                 /*                             */
       char         xtra[54];              /* Extra space (filler)        */
      } PCXHEADER;

typedef PCXHEADER    *PCXHEADERPTR;

/* Image Library Header */
typedef struct pcxlibheader
      {
       char         id[10];                /* Library ID string           */
       char         copyright[50];         /* Copyright notice            */
       unsigned     version;               /* pcxLib version              */
       char         label[40];             /* Library volume label        */
       char         xtra[20];              /* filler                      */
      } PCXLIBHEADER;

typedef PCXLIBHEADER  *PCXLIBHEADERPTR;

/* Image Library Image Header */
typedef struct pcxlibdir
      {
       char         synch;                 /* Synch byte                  */
       char         filename[13];          /* Image file name             */
       long         filesize;              /* File size                   */
       unsigned     date;                  /* File date                   */
       unsigned     time;                  /* File time                   */
       unsigned     pack;                  /* Packing type                */
       char         note[40];              /* Image note                  */
       char         xtra[20];              /* Extra filler                */
      } PCXLIBDIR;

typedef PCXLIBDIR    *PCXLIBDIRPTR;

/* PCX Toolkit Display Type Information */
typedef struct pcxdinfo
      {
       char         type;                  /* Display type                */
       char         descrip[21];           /* String description          */
       char         mode;                  /* Actual BIOS mode            */
       char         bitpx;                 /* Number of bits per pixel    */
       unsigned     hres;                  /* Horizontal resolution       */
       unsigned     vres;                  /* Vertical   resolution       */
       char         planes;                /* Number of display planes    */
       char         pages;                 /* Number of display pages     */
       unsigned     begseg;                /* Beginning display segment   */
       unsigned     pagesize;              /* Size of display page        */
       unsigned     paltype;               /* Palette format type         */
       long         dispfunc;              /* Address of display function */
       long         bufffunc;              /* Address of buffer  function */
       long         virtfunc;              /* Address of virtual function */
       long         putfunc;               /* Address of put     function */
       long         getfunc;               /* Address of get     function */
      } PCXDINFO;

typedef PCXDINFO     *PCXDINFOPTR;

/* Video Info structure */
typedef struct pcxvinfo
      {
       unsigned char  adapter;              /* Video Adapter               */
       unsigned char  display;              /* Video Display               */
      } PCXVINFO;

typedef PCXVINFO     *PCXVINFOPTR;

/* Functions */
extern  int   far pascal  pcxFileBuffer        (char far *, char far *, int);
extern  int   far pascal  pcxBufferFile        (char far *, int, char far *);
extern  int   far pascal  pcxSetDisplay        (int);
extern  int   far pascal  pcxGetDisplay        (void);
extern  int   far pascal  pcxSetMode           (int);
extern  int   far pascal  pcxSetBuffer         (char far *, int);
extern  int   far pascal  pcxBufferDisplay     (char far *, int, int, int, int);
extern  int   far pascal  pcxDisplayBuffer     (char far *, int, int, int, int, int, int);
extern  int   far pascal  pcxLibBuffer         (char far *, char far *, char far *, int);
extern  int   far pascal  pcxFileDisplay       (char far *, int, int, int);
extern  int   far pascal  pcxDisplayFile       (char far *, int, int, int, int, int);
extern  int   far pascal  pcxLibDisplay        (char far *, char far *, int, int, int);
extern  int   far pascal  pcxSetPage           (int);
extern  int   far pascal  pcxGetPage           (void);
extern  int   far pascal  pcxVerifyBuffer      (char far *);
extern  int   far pascal  pcxGetBufferHeader   (char far *, PCXHEADER far *);
extern  int   far pascal  pcxSetBufferHeader   (char far *, PCXHEADER far *);
extern  int   far pascal  pcxGetBufferType     (char far *);
extern  int   far pascal  pcxVerifyFile        (char far *);
extern  int   far pascal  pcxGetFileHeader     (char far *, PCXHEADER far *);
extern  int   far pascal  pcxSetFileHeader     (char far *, PCXHEADER far *);
extern  int   far pascal  pcxGetFileType       (char far *);
extern  int   far pascal  pcxGetLibHeader      (char far *, char far *, PCXHEADER far *);
extern  int   far pascal  pcxGetLibType        (char far *, char far *);
extern  int   far pascal  pcxDecodePalette     (int, char far *, char far *);
extern  int   far pascal  pcxEncodePalette     (int, char far *, char far *);
extern  int   far pascal  pcxSetDisplayPalette (char far *);
extern  int   far pascal  pcxGetDisplayPalette (char far *);
extern  int   far pascal  pcxSetBufferPalette  (int, char far *, int, char far *);
extern  int   far pascal  pcxGetBufferPalette  (int, char far *, int, char far *);
extern  int   far pascal  pcxSetFilePalette    (int, char far *, char far *);
extern  int   far pascal  pcxGetFilePalette    (int, char far *, char far *);
extern  int   far pascal  pcxGetLibPalette     (int, char far *, char far *, char far *);
extern  int   far pascal  pcxBeginPaletteGrab  (char far *);
extern  int   far pascal  pcxEndPaletteGrab    (void);

/* New 3.5 Functions */
extern  int   far pascal  pcxGetVersion        (char far *);
extern  int   far pascal  pcxTextBuffer        (char far *);
extern  int   far pascal  pcxBufferText        (char far *);
extern  int   far pascal  pcxVideoInfo         (PCXVINFO far *);
extern  int   far pascal  pcxQueryChipset      (void);
extern  int   far pascal  pcxGetDisplayInfo    (int, PCXDINFO far *);
extern  int   far pascal  pcxSetPrinter        (int, int, int, int, int, int);
extern  int   far pascal  pcxBufferPrint       (char far *, int, int, int);
extern  int   far pascal  pcxFilePrint         (char far *, int, int);
extern  int   far pascal  pcxLibPrint          (char far *, char far *, int, int);
extern  int   far pascal  pcxEMInstalled       (void);
extern  long  far pascal  pcxVirtualSize       (int, int, int);
extern  long  far pascal  pcxVirtualFree       (int);
extern  int   far pascal  pcxCreateVirtual     (int, long far *, int, int, int);
extern  int   far pascal  pcxDestroyVirtual    (long);
extern  int   far pascal  pcxBufferVirtual     (char far *, int, long);
extern  int   far pascal  pcxFileVirtual       (char far *, long);
extern  int   far pascal  pcxLibVirtual        (char far *, char far *, long);
extern  int   far pascal  pcxSetHerc           (int);
extern  int   far pascal  pcxModeCheck         (int);
extern  int   far pascal  pcxVirtualDisplay    (long, int, int, int, int, int, int, int);
extern  int   far pascal  pcxVirtualScroll     (long, int, int, int, int, int, int, int);
extern  int   far pascal  pcxLibSearch         (char far *, char far *, PCXLIBDIR far *, int);
extern  int   far pascal  pcxVirtualPrint      (long, int, int);
extern  int   far pascal  pcxDisplayPrint      (int, int);
extern  int   far pascal  pcxPutImage          (long, int, int, int, int);
extern  int   far pascal  pcxGetImage          (long, int, int, int, int, int);

#endif // _PCXLIB_H_

/* <-- EOF --> */