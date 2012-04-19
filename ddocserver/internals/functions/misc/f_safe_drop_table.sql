/*
  Function: f_safe_drop_table(varchar)
  Служебная функция. Используется для удаления таблиц из БГД.
  
  Parameters:
  
  $1 - название удаляемой таблицы
  
  Returns:
  
  true при успехе (т.е. указанная таблица была найдена и удалена)
  false в случае, если указанная таблица не найдена
*/
create or replace function f_safe_drop_table(varchar) returns bool as
$BODY$
declare
  table_name alias for $1;
  tcount int4;
  dyn_query varchar;
begin
  select f_is_table_exist(table_name, NULL) into tcount;
  if(tcount > 0) then
    dyn_query := 'drop table ' || table_name || ' cascade';
    execute dyn_query;
    --drop table table_name;
    return true;
  end if;
  return false;
end
$BODY$
language 'plpgsql';

/*
  Function: f_safe_drop_table(varchar)
  Служебная функция. Используется для удаления таблиц из БГД.
  
  Parameters:
  
  $1 - название удаляемой таблицы
  
  Returns:
  
  true при успехе (т.е. указанная таблица была найдена и удалена)
  false в случае, если указанная таблица не найдена
*/
create or replace function f_safe_drop_table(varchar, varchar) returns bool as
$BODY$
declare
  table_name alias for $1;
  schemaName alias for $2;

  tcount int4;
  dyn_query varchar;
begin
  select f_is_table_exist(table_name, schemaName) into tcount;
  if(tcount > 0) then
    dyn_query := 'drop table ' || table_name || ' cascade';
    execute dyn_query;
    --drop table table_name;
    return true;
  end if;
  return false;
end
$BODY$
language 'plpgsql';
