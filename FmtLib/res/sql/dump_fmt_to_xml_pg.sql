SELECT XMLSERIALIZE(
 DOCUMENT
 XMLELEMENT(NAME "Table",
   XMLELEMENT(NAME "Id", n.t_id),
   XMLELEMENT(NAME "Name", n.t_name),
   XMLELEMENT(NAME "CacheSize", n.t_cachesize),
   XMLELEMENT(NAME "Owner", n.t_owner),
   XMLELEMENT(NAME "Comment", n.t_comment),
   XMLELEMENT(NAME "PkIndex", n.t_pkidx),
   XMLELEMENT(NAME "BlobType", n.t_blobtype),
   XMLELEMENT(NAME "BlobLength", n.t_bloblen),
   XMLELEMENT(NAME "Flags", n.t_flags),
   -- Fields
   XMLELEMENT(NAME "Fields",
     (SELECT XMLAGG(
      XMLELEMENT(NAME "Field",
        XMLELEMENT(NAME "Id", f.t_id),
        XMLELEMENT(NAME "FmtId", f.t_fmtid),
        XMLELEMENT(NAME "Name", f.t_name),
        XMLELEMENT(NAME "Type", f.t_type),
        XMLELEMENT(NAME "Size", f.t_size),
        XMLELEMENT(NAME "Offset", f.t_offset),
        XMLELEMENT(NAME "OutLen", f.t_outlen),
        XMLELEMENT(NAME "DecPoint", f.t_decpoint),
        XMLELEMENT(NAME "Hidden",
          CASE WHEN f.t_hidden = chr(0) THEN 'false' ELSE 'true' END),
        XMLELEMENT(NAME "Comment", f.t_comment)
      )
    )
      FROM fmt_fields f
      WHERE f.t_fmtid = n.t_id)
   ),
   -- Keys
   XMLELEMENT(NAME "Keys",
     (SELECT XMLAGG(
      XMLELEMENT(NAME "Key",
        XMLELEMENT(NAME "FmtId", k.t_fmtid),
        XMLELEMENT(NAME "KeyNumber", k.t_keynum),
        XMLELEMENT(NAME "SegmentNumber", k.t_segnum),
        XMLELEMENT(NAME "FieldId", k.t_fmtfldid),
        XMLELEMENT(NAME "Flags", k.t_flags),
        XMLELEMENT(NAME "Type", k.t_type),
        XMLELEMENT(NAME "NullValue", k.t_nullval),
        XMLELEMENT(NAME "IsReal", k.t_isreal),
        XMLELEMENT(NAME "Comment", k.t_comment)
      )
    )
      FROM fmt_keys k
      WHERE k.t_fmtid = n.t_id)
   )
 )
 AS CLOB
) AS xml_result
FROM fmt_names n
WHERE LOWER(n.t_name) = LOWER('ExampleTableName');
-- !!!!!!!!!!!!!!!!!!! Заменить наименование на свое !!!!!!!!!!!!!!!!!!!
