/*
  Function: f_safe_drop_view(varchar)
  ��������� �������. ������������ ��� �������� ������������� �� ���.
  
  Parameters:
  
  $1 - �������� ���������� �������������
  
  Returns:
  
  true ��� ������ (�.�. ��������� ������������� ���� ������� � �������)
  false � ������, ���� ��������� ������������� �� �������
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
