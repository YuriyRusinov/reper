/*
  Function: f_is_view_exist(varchar)
  Служебная функция. Используется для проверки, существует ли указанное в качестве параметра представление в БГД.
  
  Parameters:
  
  $1 - название представления
  
  Returns:
  
  1 при успехе (т.е. указанное представление существует)
  0 в случае, если указанное представление не существует
*/
create or replace function f_is_view_exist(varchar) returns int4 as
$BODY$
declare
  iviewname alias for $1;
  tcount int4;
begin
  select count(viewname) into tcount from pg_views where viewname = iviewname and schemaname = 'public';
  return tcount;
end
$BODY$
language 'plpgsql';
