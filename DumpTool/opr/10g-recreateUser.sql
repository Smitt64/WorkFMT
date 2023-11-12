set termout on
set heading off

DROP USER &&1 CASCADE;

CREATE USER &&1 IDENTIFIED BY &&1 
    DEFAULT TABLESPACE USERS
    quota UNLIMITED on USERS;

GRANT CONNECT, SELECT_CATALOG_ROLE, CREATE TYPE, CREATE ANY TABLE, CREATE CLUSTER, 
      CREATE TRIGGER, CREATE OPERATOR, CREATE SEQUENCE, CREATE INDEXTYPE, 
      CREATE PROCEDURE, DROP ANY TABLE, ALTER SESSION, CREATE DATABASE LINK, CREATE SYNONYM, CREATE VIEW TO &&1;

GRANT EXECUTE ON DBMS_AQ TO &&1;
GRANT EXECUTE ON DBMS_AQADM TO &&1;
GRANT AQ_ADMINISTRATOR_ROLE TO &&1;
GRANT CHANGE NOTIFICATION TO &&1;

ALTER USER &&1 quota UNLIMITED on INDX;

declare 
  k number(38);
begin
  dbms_java.grant_permission( grantee => '&&1', 
                              permission_type => 'SYS:java.lang.reflect.ReflectPermission', 
                              permission_name => 'suppressAccessChecks', 
                              permission_action => '',
                              key => k );
end;
/

Exit;