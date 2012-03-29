/*
  Function: f_is_type_exist(varchar)
  Служебная функция. Используется для проверки, существует ли указанный в качестве параметра пользовательский тип данных в БГД.
  
  Parameters:
  
  $1 - название типа данных
  
  Returns:
  
  1 при успехе (т.е. указанный тип данных существует)
  0 в случае, если указанный тип данных не существует
*/
create or replace function f_is_type_exist(varchar) returns int as
$BODY$
declare
  itypname alias for $1;
  tcount int4;
begin
  select count(typname) into tcount from pg_type where typname = itypname;
  return tcount;
end
$BODY$
language 'plpgsql';
