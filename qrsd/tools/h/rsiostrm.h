/*******************************************************************************
 FILE         :   RSIOSTRM.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   некоторые компиляторы не могут включить файл iostream, а только iostream.h

 PROGRAMMED BY:   Alex Kormukhin

 CREATION DATE:   01.09.2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsiostrm.h $
                  $Revision: 1 $
                  $Date: 1.09.00 19:17 $
                  $Author: Kormukhin $
                  $Modtime: 1.09.00 19:16 $
*******************************************************************************/

#ifndef __RSIOSTRM_H
#define __RSIOSTRM_H

#ifdef _OLD_IOSTREAM_
    #include <iostream.h>
#else
    #include <iostream>
#endif

#endif /* __RSIOSTRM_H */

/* <-- EOF --> */