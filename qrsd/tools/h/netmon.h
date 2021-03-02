// Монитор сетевых соединений
// 14.12.94

#ifndef _NETMON_H_
#define _NETMON_H_

// Структура описания рабочей станции (узла)
typedef struct
      {
       char  nodeAddress[13];   // адрес станции (узла), например "   40953B941"
       char  Contenst[61];      // строка описания
      } CONTENSTNODE;

// Размер структуры описания станции

#define  CNSIZE  74

void NetMonitor(
                CONTENSTNODE *Cn,       // указатель на структуру описания
                int           Nnodes    // количество записей
               );

#endif // _NETMON_H_

/* <-- EOF --> */