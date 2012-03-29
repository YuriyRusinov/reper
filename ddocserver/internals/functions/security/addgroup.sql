/*
  Function: addGroup(name, varchar, boolean)

  �������� ������ �������������. 
  
  ���� ������ � ����� ��������� ��� ���������� � �������� ��� ������, �� ��� ������ ����������� � ������� GROUPS.

  Parameters:
  $1 - �������� ������ (������������ ��������. �� ����� ���� NULL)
  
  $2 - ���� ������������ ���� �����, � ������� ������ ������ ������ (���� TRUE, �� ������ �����������)
  
  Returns:
  ������������� ��������� ������.
  
  ���� ������� ��������� ������� ��� NULL ��� ��� �������� ������ ��������� ������, �� ������� ���������� ����������.
*/
CREATE OR REPLACE FUNCTION addGroup(name, boolean) RETURNS INTEGER AS
$BODY$
DECLARE
    gn ALIAS FOR $1;
    needInherit alias for $2;

    dyn_query VARCHAR;
    gid int4;
    res int4;
BEGIN
    if(gn isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;
    
    --������� ������ � ������� pg_group
    select safeCreateGroup(gn) into res;
    if(res = 0) then
	select count(groname) into res from pg_group where groname = gn;
	if(res = 1) then 
	    raise exception 'Clucter already contains ROLE with specified name. New GROUP must have another name!';
	end if;
    end if;
    
    RETURN res;
END;
$BODY$
LANGUAGE 'plpgsql';
