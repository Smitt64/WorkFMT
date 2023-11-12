SET ORACLE_SID={ORACLE_SID}

:: Имя пользователя владельца схемы 
set FROMUSER={FROMUSER}

:: Имя и пароль пользователя, под правами которого заливаем
set SYSUSER={SYSUSER}
set SYSPASS={SYSPASS}

:: Имя и пароль пользователя, в которого поднимаем данные
set TOUSER={TOUSER}
set TOPASS={TOPASS}

:: Имена файлов (дамп и лог)
set DUMPFI={FROMUSER}.DMP
set LOGFI={FROMUSER}.log

set v_parallel=2

:: Импорт дампа
impdp %SYSUSER%/%SYSPASS%@%ORACLE_SID% directory=IMPDP_DIR1 dumpfile=%DUMPFI% logfile=%LOGFI% remap_schema=%FROMUSER%:%TOUSER%  TABLE_EXISTS_ACTION=TRUNCATE parallel=%v_parallel% TRANSFORM=oid:n