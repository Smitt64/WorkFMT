chcp 866

:: ���짮��⥫� ��
set user=

:: ��஫� ���짮��⥫� ��
set pass=

:: TNS
set tns=

set NLS_LANG=AMERICAN_AMERICA.RU8PC866

for %%G in (*.sql) do exit | sqlplus -S %user%/%pass%@%tns% @"%%G" > .\log\"%%G".log

pause
