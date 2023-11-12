SET ORACLE_SID={ORACLE_SID}

:: ��� ���짮��⥫� �������� �奬� 
set FROMUSER={FROMUSER}

:: ��� � ��஫� ���짮��⥫�, ��� �ࠢ��� ���ண� ��������
set SYSUSER={SYSUSER}
set SYSPASS={SYSPASS}

:: ��� � ��஫� ���짮��⥫�, � ���ண� ��������� �����
set TOUSER={TOUSER}
set TOPASS={TOPASS}

:: ����� 䠩��� (���� � ���)
set DUMPFI={FROMUSER}.DMP
set LOGFI={FROMUSER}.log

set v_parallel=2

:: ������ �����
impdp %SYSUSER%/%SYSPASS%@%ORACLE_SID% directory=IMPDP_DIR1 dumpfile=%DUMPFI% logfile=%LOGFI% remap_schema=%FROMUSER%:%TOUSER%  TABLE_EXISTS_ACTION=TRUNCATE parallel=%v_parallel% TRANSFORM=oid:n