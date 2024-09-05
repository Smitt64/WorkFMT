DECLARE
	CURSOR c IS
	select 
		utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,1))     ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,2001))  ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,4001))  ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,6001))  ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,8001))  ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,10001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,12001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,14001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,16001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,18001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,20001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,22001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,24001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,26001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,28001)) ||
        utl_raw.cast_to_varchar2(DBMS_LOB.SUBSTR(t_fmtblobdata_xxxx,2000,30001))
	blb from %1
	for update;
BEGIN
	FOR i IN c LOOP
		update %1 set t_fmtblobdata_xxxx = i.blb
		where current of c;
	END LOOP;
	COMMIT;
END;
/