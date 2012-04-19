/*
  Function: f_safe_drop_table(varchar)
  ��������� �������. ������������ ��� �������� ������ �� ���.
  
  Parameters:
  
  $1 - �������� ��������� �������
  
  Returns:
  
  true ��� ������ (�.�. ��������� ������� ���� ������� � �������)
  false � ������, ���� ��������� ������� �� �������
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
  ��������� �������. ������������ ��� �������� ������ �� ���.
  
  Parameters:
  
  $1 - �������� ��������� �������
  
  Returns:
  
  true ��� ������ (�.�. ��������� ������� ���� ������� � �������)
  false � ������, ���� ��������� ������� �� �������
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
