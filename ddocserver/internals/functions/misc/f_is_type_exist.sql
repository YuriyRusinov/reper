/*
  Function: f_is_type_exist(varchar)
  ��������� �������. ������������ ��� ��������, ���������� �� ��������� � �������� ��������� ���������������� ��� ������ � ���.
  
  Parameters:
  
  $1 - �������� ���� ������
  
  Returns:
  
  1 ��� ������ (�.�. ��������� ��� ������ ����������)
  0 � ������, ���� ��������� ��� ������ �� ����������
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
