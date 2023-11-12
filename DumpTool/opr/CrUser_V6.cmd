@echo off
sqlplus %1/%2@%3 @10g-recreateUser.sql %4 %4 USERS INDX