/*
  Function: f_safe_drop_type(varchar)
  ��������� �������. ������������ ��� �������� ������� ���������������� ����� ������ �� ���.
  
  Parameters:
  
  $1 - �������� ���������� ���� ������
  
  Returns:
  
  true ��� ������ (�.�. ��������� ��� ������ ��� ������ � ������)
  false � ������, ���� ��������� ��� ������ �� ������
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
