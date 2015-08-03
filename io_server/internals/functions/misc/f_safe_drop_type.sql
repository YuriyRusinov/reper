/*
  Function: f_safe_drop_type(varchar)
  Служебная функция. Используется для удаления сложных пользовательских типов данных из БГД.
  
  Parameters:
  
  $1 - название удаляемого типа данных
  
  Returns:
  
  true при успехе (т.е. указанный тип данных был найден и удален)
  false в случае, если указанный тип данных не найден
*/
create or replace function f_safe_drop_type(varchar) returns bool as
$BODY$
declare
  type_name alias for $1;
  tcount int4;
  dyn_query varchar;
begin
  select f_is_type_exist(type_name) into tcount;
  if(tcount > 0) then
    dyn_query := 'drop type ' || type_name || ' cascade';
    execute dyn_query;
    --drop type type_name;
    return true;
  end if;
  return false;
end
$BODY$
language 'plpgsql';
