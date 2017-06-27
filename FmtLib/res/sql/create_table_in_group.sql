CREATE TABLE "table_in_group" (
"group_uid"  TEXT(39) NOT NULL,
"table_name"  TEXT(30) NOT NULL,
UNIQUE ("group_uid", "table_name")
);
