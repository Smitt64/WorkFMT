begin
-- #conv-Oracle
DECLARE
	TYPE nested_type IS TABLE OF VARCHAR2(150);
	upgrade_object nested_type;
BEGIN
	upgrade_object := nested_type
	(
		%1
	);

	FOR i IN upgrade_object.FIRST .. upgrade_object.LAST 
	LOOP
		BEGIN
			EXECUTE IMMEDIATE 'ALTER TRIGGER ' || upgrade_object(i) || ' COMPILE';
			EXECUTE IMMEDIATE 'ALTER TRIGGER ' || upgrade_object(i) || ' ENABLE';
		EXCEPTION
			WHEN OTHERS THEN NULL;
		END;
	END LOOP;
	upgrade_object.DELETE;
END;
-- #conv-PG
--DO $$
--DECLARE
--    upgrade_object TEXT[] := ARRAY[
--        %2
--    ];
--    trigger_name TEXT;
--BEGIN
--    FOREACH trigger_name IN ARRAY upgrade_object LOOP
--        BEGIN
--            EXECUTE format('ALTER TABLE %3 ENABLE TRIGGER %I', lower(trigger_name));
--        EXCEPTION
--            WHEN OTHERS THEN NULL;
--        END;
--    END LOOP;
--END $$;
-- #conv-end  
end;
/