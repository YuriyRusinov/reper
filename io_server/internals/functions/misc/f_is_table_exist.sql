/*
  Function: f_is_table_exist(varchar)
  Служебная функция. Используется для проверки, существует ли указанная в качестве параметра таблица в БГД.
  
  Parameters:
  
  $1 - название таблицы
  
  Returns:
  
  1 при успехе (т.е. указанная таблица существует)
  0 в случае, если указанная таблица не существует
*/
create or replace function f_is_table_exist(varchar, varchar) returns int4 as
$BODY$
declare
  itablename alias for $1;
  iSchemaName alias for $2;

  tcount int4;
  vSchemaName varchar;
begin
  if(iSchemaName isnull or iSchemaName = '') then
      vSchemaName = 'public';
  else
      vSchemaName = iSchemaName;
  end if;

  select count(tablename) into tcount from pg_tables where tablename = itablename and schemaname = vSchemaName ;

  return tcount;

end
$BODY$
language 'plpgsql';

create or replace function f_is_temp_table_exist(varchar) returns int4 as
$BODY$
declare
  itablename alias for $1;
  tcount int4;
  q varchar;
begin

  q = 'create temp table ' || itablename || ' (id int4)';
  execute q;

  q = 'drop table ' || itablename;
  execute q;

  return 0;

EXCEPTION
        WHEN duplicate_table THEN
            RETURN 1;
end
$BODY$
language 'plpgsql';
