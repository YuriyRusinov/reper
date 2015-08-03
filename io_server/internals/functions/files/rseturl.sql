/*
  Function: rSetUrl(int4, varchar)
  ������� ������ ����� URL �������� �����. �.�. ����� ������ ���������, ��� ����, ����������� � ������ ��������, �������� �� ��������� ����. 
  ������ ���������� ���� ������������ ���� � ��������� ���������.
  
  Parameters:
 
  $1 - ������������� �������� ������
  
  $2 - ����� URL
  
  Returns:
  1 ��� ������
  
  0, ���� ������ �������� �������� ������ (��� ��������� ������������)
  
  -1, ���� ������������ �� ����� ���� �� ���������� ���������� � �������� �������� ���������.
  
  -2, ���� ������ �������� ������������� ��������
*/
create or replace function rSetUrl(int4, varchar) returns int4 as
$BODY$
declare
	iid_file alias for $1;
	iurl alias for $2;
	
	r RECORD;
	tableName varchar;
	dyn_query varchar;
	
	idUrl int4;	
begin

	if(iurl isnull) then
		return 0;
	end if;

	select id into idUrl from io_urls where id = iid_file;
	if(idUrl isnull) then
            return -2;
        end if;
  
	update io_urls set url = iurl where id = idUrl;
	
	return  1;
  
end
$BODY$
language 'plpgsql' security definer;
