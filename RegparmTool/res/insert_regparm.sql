DECLARE
  v_dregparm_dbt__T_KEYID dregparm_dbt.T_KEYID%TYPE;
  FUNCTION InsertIntoRegparm__ (p_PARENTID IN NUMBER, p_NAME IN VARCHAR2, p_TYPE IN NUMBER, p_GLOBAL IN CHAR, p_DESCRIPTION IN VARCHAR2, p_SECURITY IN CHAR, p_ISBRANCH IN CHAR, p_TEMPLATE IN VARCHAR2) RETURN DREGPARM_DBT.T_KEYID%TYPE
  AS
  -- #conv-Oracle
     ErrorText VARCHAR2(2000) := CHR(1);
  -- #conv-PG
  -- DECLARE
  -- ErrorText varchar(2000);
  -- #conv-end 
    v_dregparm_dbt__T_KEYID dregparm_dbt.T_KEYID%TYPE;
  BEGIN
    SELECT NVL(MAX(T_KEYID), 0) + 1 INTO v_dregparm_dbt__T_KEYID FROM DREGPARM_DBT;
  
    INSERT INTO dregparm_dbt(T_KEYID,T_PARENTID,T_NAME,T_TYPE,T_GLOBAL,T_DESCRIPTION,T_SECURITY,T_ISBRANCH,T_TEMPLATE) VALUES (v_dregparm_dbt__T_KEYID, p_PARENTID, p_NAME, p_TYPE, p_GLOBAL, p_DESCRIPTION, p_SECURITY, p_ISBRANCH, p_TEMPLATE);
  
    RETURN v_dregparm_dbt__T_KEYID;
  EXCEPTION 
  WHEN DUP_VAL_ON_INDEX THEN 
    -- #conv-Oracle
    ErrorText := Substr(sqlerrm, 11);
    -- #conv-PG
    -- ErrorText := sqlerrm;
    -- #conv-end 
    dbms_output.put_line('%WARNING%: Ошибка вставки в таблицу DREGPARM_DBT: ' || ErrorText);
    dbms_output.put_line('(t_parentid,t_name) = (' || p_PARENTID || ', ' || p_NAME || ')');
    RETURN -1;
  WHEN OTHERS THEN
    -- #conv-Oracle
    ErrorText := Substr(sqlerrm, 11);
    -- #conv-PG
    -- ErrorText := sqlerrm;
    -- #conv-end 
    dbms_output.put_line('%WARNING%: Ошибка вставки в таблицу DREGPARM_DBT: ' || ErrorText);
    dbms_output.put_line('(t_parentid,t_name) = (' || p_PARENTID || ', ' || p_NAME || ')');
    RETURN -1;
  END;
  
  PROCEDURE InsertIntoRegval__ (p_KEYID IN NUMBER, p_REGKIND IN NUMBER, p_OBJECTID IN NUMBER, p_BLOCKUSERVALUE IN CHAR, p_EXPDEP IN NUMBER, p_LINTVALUE IN NUMBER, p_LDOUBLEVALUE IN FLOAT, p_FMTBLOBDATA_XXXX IN BLOB)
  AS
  -- #conv-Oracle
     ErrorText VARCHAR2(2000) := CHR(1);
  -- #conv-PG
  -- DECLARE
  -- ErrorText varchar(2000);
  -- #conv-end 
  BEGIN
      INSERT INTO DREGVAL_DBT(T_KEYID,T_REGKIND,T_OBJECTID,T_BLOCKUSERVALUE,T_EXPDEP,T_LINTVALUE,T_LDOUBLEVALUE,T_FMTBLOBDATA_XXXX) VALUES (p_KEYID, p_REGKIND, p_OBJECTID, p_BLOCKUSERVALUE, p_EXPDEP, p_LINTVALUE, p_LDOUBLEVALUE, p_FMTBLOBDATA_XXXX);
  EXCEPTION 
  WHEN DUP_VAL_ON_INDEX THEN
    -- #conv-Oracle
    ErrorText := Substr(sqlerrm, 11);
    -- #conv-PG
    -- ErrorText := sqlerrm;
    -- #conv-end 
    dbms_output.put_line('%WARNING%: Ошибка вставки в таблицу DREGVAL_DBT: ' || ErrorText);
    dbms_output.put_line('(t_keyid,t_regkind,t_objectid) = (' || p_KEYID || ', ' || p_REGKIND || ', ' || p_OBJECTID || ')');
  WHEN OTHERS THEN
    -- #conv-Oracle
    ErrorText := Substr(sqlerrm, 11);
    -- #conv-PG
    -- ErrorText := sqlerrm;
    -- #conv-end 
    dbms_output.put_line('%WARNING%: Ошибка вставки в таблицу DREGVAL_DBT: ' || ErrorText);
    dbms_output.put_line('(t_keyid,t_regkind,t_objectid) = (' || p_KEYID || ', ' || p_REGKIND || ', ' || p_OBJECTID || ')');
  END;
  
  FUNCTION GetKeyIDByPath__ (pPath IN VARCHAR2) RETURN  NUMBER
  AS
    v_dregparm_dbt_T_KEYID NUMBER := 0;
  BEGIN
    -- #conv-Oracle
      WITH path_parts AS (
     SELECT REGEXP_SUBSTR(pPath, '[^/]+', 1, LEVEL) AS part, LEVEL AS lvl
     FROM dual CONNECT BY REGEXP_SUBSTR(pPath, '[^/]+', 1, LEVEL) IS NOT NULL),
     hierarchy(keyid, parentid, name, lvl, full_path) AS (
     SELECT t.T_KEYID, t.T_PARENTID, t.T_NAME, 1, t.T_NAME
     FROM dregparm_dbt t WHERE UPPER(t.T_NAME) = UPPER((SELECT part FROM path_parts WHERE lvl = 1)) AND t.T_PARENTID = 0
     UNION ALL
     SELECT t.T_KEYID, t.T_PARENTID, t.T_NAME, h.lvl + 1, h.full_path || '/' || t.T_NAME
     FROM dregparm_dbt t JOIN hierarchy h ON t.T_PARENTID = h.keyid
     WHERE UPPER(t.T_NAME) = UPPER((SELECT part FROM path_parts WHERE lvl = h.lvl + 1)) )
     SELECT keyid INTO v_dregparm_dbt_T_KEYID FROM hierarchy WHERE lvl = (SELECT MAX(lvl) FROM path_parts) AND UPPER(full_path) = UPPER(pPath);
    -- #conv-PG
  --    WITH RECURSIVE path_parts AS (SELECT split_part(pPath, '/', n) AS part, n AS lvl
  --   FROM generate_series(1, array_length(string_to_array(pPath, '/'), 1)) AS n), hier AS (
  --   SELECT t.T_KEYID, t.T_PARENTID, t.T_NAME, 1 AS lvl
  --   FROM dregparm_dbt t WHERE t.T_NAME = (SELECT part FROM path_parts WHERE lvl = 1) AND t.T_PARENTID = 0
  --   UNION ALL
  --   SELECT t.T_KEYID, t.T_PARENTID, t.T_NAME, h.lvl + 1 FROM dregparm_dbt t JOIN hier h ON t.T_PARENTID = h.T_KEYID
  --   JOIN path_parts p ON p.lvl = h.lvl + 1 AND p.part = t.T_NAME)
  --   SELECT T_KEYID INTO STRICT v_dregparm_dbt_T_KEYID FROM hier WHERE lvl = (SELECT MAX(lvl) FROM path_parts);
    -- #conv-end
    RETURN v_dregparm_dbt_T_KEYID;
  EXCEPTION WHEN NO_DATA_FOUND THEN RETURN -1;
  END;

  FUNCTION InsertIntoRegparmPath__ (pPath IN VARCHAR2, pNAME IN VARCHAR2, pTYPE IN NUMBER, pGLOBAL IN CHAR, pDESCRIPTION IN VARCHAR2, pSECURITY IN CHAR, pISBRANCH IN CHAR, pTEMPLATE IN VARCHAR2) RETURN  NUMBER
  AS
    v_dregparm_dbt_T_KEYID NUMBER := 0;
  BEGIN
    v_dregparm_dbt_T_KEYID := GetKeyIDByPath__ (pPath);

    IF v_dregparm_dbt_T_KEYID > 0 THEN 
      v_dregparm_dbt_T_KEYID := InsertIntoRegparm__ (v_dregparm_dbt_T_KEYID, pNAME, pTYPE, pGLOBAL, pDESCRIPTION, pSECURITY, pISBRANCH, pTEMPLATE);
    END IF;

    RETURN v_dregparm_dbt_T_KEYID;
  END;

BEGIN
  rsb_ainc.disable_trigger('DREGPARM_DBT');

  %1

  rsb_ainc.enable_trigger('DREGPARM_DBT');
  rsb_ainc.restore_seq('DREGPARM_DBT');
END;
/