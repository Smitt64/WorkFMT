@echo off

rem
rem �������� 䠩� ������� �ਯ⮢ ������㥬�� �⨫�⮩ RSexp � ����
rem ������� �. 20.08.2004
rem ��ࠬ���� ���ਭ����� �१ �஡��, UserName Password TNSServiceName TableName. �ਬ�� �맮��:
rem FillTable.cmd scott tiger rsbank dabode_dbt
rem 

rem
rem ��⠭�������� ࠧ����⥫� 楫�� � �஡��� ��� � ����஢��
rem

set NLS_NUMERIC_CHARACTERS=.,
set NLS_LANG=AMERICAN_CIS.RU8PC866

rem
rem ��頥� ⠡����, �᫨ ���� �⪫�砥� ��⮨��६���� �ਣ���, ����᪠�� �ਯ� ������� ������,
rem ����砥� �ਣ���, ����⠭�������� ᨪ����.
rem

sqlplus %1/%2@%3 @FillTable.sql %~n4 %4 %1 %2 %3


rem �ਬ�� �맮��:
rem filltable.cmd pozd_3122  pozd_3122 DB11G_IRONHIDE DUFO_ACTIONLIST_DBT
rem PAUSE

