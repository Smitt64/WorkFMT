SELECT XMLSERIALIZE(
 DOCUMENT
 XMLELEMENT("Table",
   XMLFOREST(
     n.T_ID AS "Id",
     n.T_NAME AS "Name",
     n.T_CACHESIZE AS "CacheSize",
     n.T_OWNER AS "Owner",
     n.T_COMMENT AS "Comment",
     n.T_PKIDX AS "PkIndex",
     n.T_BLOBTYPE AS "BlobType",
     n.T_BLOBLEN AS "BlobLength",
     n.T_FLAGS AS "Flags"
   ),
   -- Fields
   XMLELEMENT("Fields",
     (SELECT XMLAGG(
        XMLELEMENT("Field",
          XMLFOREST(
            f.T_ID AS "Id",
            f.T_FMTID AS "FmtId",
            f.T_NAME AS "Name",
            f.T_TYPE AS "Type",
            f.T_SIZE AS "Size",
            f.T_OFFSET AS "Offset",
            f.T_OUTLEN AS "OutLen",
            f.T_DECPOINT AS "DecPoint",
            CASE WHEN f.T_HIDDEN = chr(0) THEN 'false' ELSE 'true' END "Hidden",
            f.T_COMMENT AS "Comment"
          )
        )
      )
      FROM FMT_FIELDS f
      WHERE f.T_FMTID = n.T_ID)
   ),
   -- Keys
   XMLELEMENT("Keys",
     (SELECT XMLAGG(
        XMLELEMENT("Key",
          XMLFOREST(
            k.T_FMTID AS "FmtId",
            k.T_KEYNUM AS "KeyNumber",
            k.T_SEGNUM AS "SegmentNumber",
            k.T_FMTFLDID AS "FieldId",
            k.T_FLAGS AS "Flags",
            k.T_TYPE AS "Type",
            k.T_NULLVAL AS "NullValue",
            k.T_ISREAL AS "IsReal",
            k.T_COMMENT AS "Comment"
          )
        )
      )
      FROM FMT_KEYS k
      WHERE k.T_FMTID = n.T_ID)
   )
 )
 AS CLOB
) AS xml_result
FROM FMT_NAMES n
WHERE LOWER(n.t_name) = LOWER('ExampleTableName')
-- !!!!!!!!!!!!!!!!!!! Заменить наименование на свое !!!!!!!!!!!!!!!!!!!
