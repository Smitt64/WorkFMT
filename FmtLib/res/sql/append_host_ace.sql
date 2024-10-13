
BEGIN
DBMS_NETWORK_ACL_ADMIN.APPEND_HOST_ACE
(
host => '%1',
lower_port => null,
upper_port => null,
ace => xs$ace_type(privilege_list => xs$name_list('jdwp'),
principal_name => '%2',
principal_type => xs_acl.ptype_db)
);
END;
