/*
  Function: f_safe_drop_index(varchar)
  ��������� �������. ������������ ��� �������� �������� �� ���.
  
  Parameters:
  
  $1 - �������� ���������� �������
  
  Returns:
  
  true ��� ������ 
  false � ������, ���� ��������� ������ �� ������
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
