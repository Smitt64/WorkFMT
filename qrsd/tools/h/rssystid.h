// -------------------------------------------------------------------------------------------------
// 2013-04-03  Идентификаторы инструментальных подсистем, используемые для формирования
//             ошибок (rserror.h).

#ifndef _RSSYSTID_H_
#define _RSSYSTID_H_

// -----------------------------------------------------------------------------
// Для инструментального поддиапазона предполагается следующее распределение:
//       0         - общеинструментальная ошибка
//      10 -   19  - компоненты rstools
//      20 -   29  - компоненты rsscript (в т.ч. и RSL)
//      30 -   49  - компоненты rscomserv
//      50 -   69  - компоненты DLM
//      70 -   79  - компоненты rsadmin
//      80 -   99  - компоненты utils
//     100 -  139  - компоненты server (СП и Терминал, исключая утилиты администрирования)
//     500 -  511  - резерв для расширений


// -----------------------------------------------------------------------------
// Компоненты rstools
#define  RSTID_RSTools_Tools      10
#define  RSTID_RSTools_Panel      11
#define  RSTID_RSTools_Bedit      12

// -----------------------------------------------------------------------------
// Компоненты СП и Терминала
#define  RSTID_AppServ_AppSrv    100      // Сервер приложений
#define  RSTID_AppServ_6678t     101
#define  RSTID_AppServ_DesKey    102
#define  RSTID_AppServ_RComCnt   103
#define  RSTID_AppServ_Rsc       104
#define  RSTID_AppServ_Config    105
#define  RSTID_AppServ_TstHsp    106

#define  RSTID_AppServ_Adm       120
#define  RSTID_AppServ_PngClnt   121
#define  RSTID_AppServ_Rsm       122
#define  RSTID_AppServ_Term      123      // Терминал

#endif // _RSSYSTID_H_

/* <-- EOF --> */