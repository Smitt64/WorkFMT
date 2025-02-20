:: Пользователь БД
set user=

:: Пароль пользователь БД
set pass=

:: ODBC DSN
set dsn=

set connstring="dsn="%dsn%";user id="%user%";password="%pass%

fmtxml /d /i -cs:%connstring% .\XML

pause
