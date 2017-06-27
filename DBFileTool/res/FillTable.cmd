@echo off

rem
rem Командный файл заливки скриптов генерируемых утилитой RSexp в базу
rem Новиков А. 20.08.2004
rem Параметры воспринимает через пробел, UserName Password TNSServiceName TableName. Пример вызова:
rem FillTable.cmd scott tiger rsbank dabode_dbt
rem 

rem
rem Устанавливаем разделитель целой и дробной части и кодировку
rem

set NLS_NUMERIC_CHARACTERS=.,
set NLS_LANG=AMERICAN_CIS.RU8PC866

rem
rem Очищаем таблицу, если надо отключаем автоинкрементный триггер, запускаем скрипт заливки данных,
rem включаем триггер, восстанавливаем сиквенс.
rem

sqlplus %1/%2@%3 @FillTable.sql %~n4 %4 %1 %2 %3


rem Пример вызова:
rem filltable.cmd pozd_3122  pozd_3122 DB11G_IRONHIDE DUFO_ACTIONLIST_DBT
rem PAUSE

