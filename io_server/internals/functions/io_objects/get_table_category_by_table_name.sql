CREATE OR REPLACE FUNCTION get_table_category_by_table_name(character varying)
  RETURNS integer AS
$BODY$
declare
    tableName alias for $1;
    r integer;
begin
    select id_child into r from io_categories where id=(select id_io_category from io_objects where table_name=tableName);
    return r;
end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION get_table_category_by_table_name(character varying)
  OWNER TO yrusinov;

