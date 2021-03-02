// -------------------------------------------------------------------------------------------------
// 2013-05-16  �⨫��� �㭪樨 ��� ࠡ��� � ��⠫�����

#ifndef _RSUTILDIR_H_
#define _RSUTILDIR_H_

#include <string>
#include <vector>


// -----------------------------------------------------------------------------
namespace RSUDIR
        {
         // --------------------------------------------------------------------
         // ���ᨢ ��᮪ 䠩���
         typedef std::vector<std::string>  TVFileMask;


         // --------------------------------------------------------------------
         // ����砥� ���� (��� ����� ��᮪)
         // [out] maskOnly - ��᪨ �⡮� ("*.mac,*.lst")
         // [in]  fullPath - ����� ���� � ��᪠�� ("D:\Test\*.mac,*.lst")
         // [in]  path     - ��⠫�� �� fullPath ("\Test\")
         void ExtractMask(char *maskOnly, const char *fullPath, const char *path);

         // --------------------------------------------------------------------
         // ������塞 ���ᨢ ��᪠��
         // [out] lst    - ���ᨢ ��ப (string) � ��᪠��
         // [in]  masks  - ��ப� � ��᪠�� ("*.mac,*.lst")
         // [in]  delims - ᨬ����-ࠧ����⥫� ����� ��᪠�� � masks
         void CreateMaskArray(TVFileMask &lst, const char *masks, const char *delims = ",;");
        };

#endif _RSUTILDIR_H_

/* <-- EOF --> */