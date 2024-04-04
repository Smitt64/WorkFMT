DO  
$body$
BEGIN
    CREATE ROLE %1;
EXCEPTION WHEN others THEN
    RAISE NOTICE '%1 role exists, not re-creating';
END
$body$;