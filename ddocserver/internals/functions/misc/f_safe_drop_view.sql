/*
  Function: f_safe_drop_view(varchar)
  Служебная функция. Используется для удаления представлений из БГД.
  
  Parameters:
  
  $1 - название удаляемого представления
  
  Returns:
  
  true при успехе (т.е. указанное представление было найдено и удалено)
  false в случае, если указанное представление не найдено
*/
create or replace function f_safe_drop_view(varchar) returns bool as
$BODY$
declare
    view_name alias for $1;
    tcount int4;
    dyn_query varchar;
begin

    select f_is_view_exist(view_name) into tcount;
    if(tcount > 0) then
        dyn_query := 'drop view ' || view_name || ' cascade';
        execute dyn_query;
        return true;
    end if;
  
    return false;
end
$BODY$
language 'plpgsql';
