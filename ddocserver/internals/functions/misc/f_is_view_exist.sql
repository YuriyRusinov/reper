/*
  Function: f_is_view_exist(varchar)
  ��������� �������. ������������ ��� ��������, ���������� �� ��������� � �������� ��������� ������������� � ���.
  
  Parameters:
  
  $1 - �������� �������������
  
  Returns:
  
  1 ��� ������ (�.�. ��������� ������������� ����������)
  0 � ������, ���� ��������� ������������� �� ����������
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
