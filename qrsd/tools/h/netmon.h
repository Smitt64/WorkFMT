// ������ �⥢�� ᮥ�������
// 14.12.94

#ifndef _NETMON_H_
#define _NETMON_H_

// ������� ���ᠭ�� ࠡ�祩 �⠭樨 (㧫�)
typedef struct
      {
       char  nodeAddress[13];   // ���� �⠭樨 (㧫�), ���ਬ�� "   40953B941"
       char  Contenst[61];      // ��ப� ���ᠭ��
      } CONTENSTNODE;

// ������ �������� ���ᠭ�� �⠭樨

#define  CNSIZE  74

void NetMonitor(
                CONTENSTNODE *Cn,       // 㪠��⥫� �� �������� ���ᠭ��
                int           Nnodes    // ������⢮ ����ᥩ
               );

#endif // _NETMON_H_

/* <-- EOF --> */