/*
  Function: f_safe_drop_index(varchar)
  Служебная функция. Используется для удаления индексов из БГД.
  
  Parameters:
  
  $1 - название удаляемого индекса
  
  Returns:
  
  true при успехе 
  false в случае, если указанный индекс не найден
*/
create or replace function f_safe_drop_index(varchar) returns bool as
$BODY$
declare
  indexName alias for $1;
  tcount int4;
  dyn_query varchar;
begin
  select f_is_index_exist(indexName) into tcount;
  if(tcount > 0) then
    dyn_query := 'drop index ' || indexName || ' ';
    execute dyn_query;
    --drop type type_name;
    return true;
  end if;
  return false;
end
$BODY$
language 'plpgsql';
