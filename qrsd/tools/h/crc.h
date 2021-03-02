/**************************************************************************
 * Расчет контрольной суммы                   *
 * Оригинальная идея Alexander V. Ivanov 1992                             *
 * Переработка под нужды RS-Bank Konstantin V. Andreev 1996        *
 *                                                                        *
 * CRC.C - CRC table and calculate routine                            *
 *
 * VERSION INFO :    $Archive: /RS-Tools/Old_tools/Tools/H/crc.h $
                     $Revision: 5 $
                     $Date: 4.06.98 16:59 $
                     $Author: Markovitch $
                     $Modtime: 27.05.98 23:59 $
*******************************************************************************/

#ifndef __CRC_H
#define __CRC_H
#define _CRCH_

#include "platform.h"
#include "tooldef.h"

#include <stddef.h>

typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned long   dword;

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
extern  dword RSL_FAR _TOOLEXP crc_32_tab[];

// -----------------------------------------------------------------------------
#define  add_CRC_32(a, b)  (crc_32_tab[((byte)a ^ b) & 0xff] ^ (a >> 8))

// -----------------------------------------------------------------------------
_TOOLEXP dword get_CRC_32(const void *buffer, size_t length);

// Аналог get_CRC_32, но позволяет вычислять CRC для разрывных областей
_TOOLEXP dword get_CRC_32_Ex(dword startCRC, const void *buffer, size_t length);

#ifdef __cplusplus
} ;
#endif

#endif

/* <-- EOF --> */