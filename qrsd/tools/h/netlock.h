//***********************************************************************
//   Group locks for NetWare applications.
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/netlock.h $
//                  $Revision: 7 $
//                  $Date: 20.03.00 20:11 $
//                  $Author: Lebedev $
//                  $Modtime: 17.03.00 14:24 $
//***********************************************************************

// October 13,1994     Кубрин С. (K78)
//               Убрал включение nwcalls.h
// 22.10.99 Полностью переведено на альтернативную регистрацию

#ifndef __NETLOCK_H
#define __NETLOCK_H

#include "apptldef.h"
#include "fs.h"

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
typedef void (* SAddrFunc_t)(unsigned char *);

extern _APPTLDATA SAddrFunc_t  SAddrFuncPtr;  // Указатель на функцию получения адреса 

extern _APPTLDATA unsigned int   CHandle;
extern _APPTLDATA unsigned int   CNum;
extern _APPTLDATA unsigned char  networkNumber[4];
extern _APPTLDATA unsigned char  SAddr[6];
extern _APPTLDATA unsigned int   socketNumber;


// -----------------------------------------------------------------------------
// Установить текущие сетевые параметры
// Если в качестве параметра data передать NULL, сетевые параметры клиента будут
// запрошены с помощью функции fs_getTerminalNetInfo.
// [in]  data - указатель на структуру, содержащую новые значения параметров
_APPTLFUNC void SetClientNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// Установить текущие сетевые параметры
// Если в качестве параметра data передать NULL, сетевые параметры клиента будут
// запрошены с помощью функции fs_getTerminalNetInfo2.
// [in]  data - указатель на структуру, содержащую новые значения параметров
_APPTLFUNC void SetClientNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// Получить текущие сетевые параметры
// [out]  data - указатель на структуру-назначение
_APPTLFUNC void GetClientNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// Получить текущие сетевые параметры
// [out]  data - указатель на структуру-назначение
_APPTLFUNC void GetClientNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// Получить локальные сетевые параметры
// [out]  data - указатель на структуру-назначение
_APPTLFUNC void GetLocalNetInfo(TClientNetInfo *data);

// -----------------------------------------------------------------------------
// Получить локальные сетевые параметры
// [out]  data - указатель на структуру-назначение
_APPTLFUNC void GetLocalNetInfo2(TClientNetInfo2 *data);

// -----------------------------------------------------------------------------
// Регистрация работы приложения в системе (в базе данных)
// Значение параметра grn = "OFF" отменяет регистрацию
_APPTLFUNC void initNetwGr    // Network group initialisation.
                          (
                           char *grn                // Group name. Truncated to 8 characters!
                          );


// Функция разрегистрации. Вызывать без надобности не надо, поскольку она становится
// atexit'овой функцией при выполнении initNetwGr
_APPTLFUNC void ClearConnection(void);


// Получение или снятие монопольного доступа (GrLock( OFF ) эквивалентно
// вызову ClearGrLock
_APPTLFUNC unsigned char GrLock(                         // 0 - OK  1 - Failed  2 - Network error
                                unsigned char excl       // ON - Full lock needed
                               );

// Снятие монопольной работы (разблокирование группы)
_APPTLFUNC void ClearGrLock(void);

// Показать существующие подсоединения
_APPTLFUNC void ShowConnection(void);

// Обновить список подсоединений
_APPTLFUNC void ReloadConnection(void);

#ifdef __cplusplus
}
#endif

#endif // __NETLOCK_H

/* <-- EOF --> */