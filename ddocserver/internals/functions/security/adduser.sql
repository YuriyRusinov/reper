/*
  Function: addUser(name, name, varchar, name, boolean)
  
  ���������� ������ ������������ � �������. ������������ ��������� ��� �� ������� ����� �� ����� �������� � ��������� ��. 
  ����������� ����� ������� �������� ��������, ���������� ���������������� ��� ����� ���������. 
  
  ���� ������������ � �������� ������ ��� ���������� � �������� ��� ������, �� �� �� ���������, � ���� ����������� � ������� USERS � � ��������� ������.
  
  Parameters:
  $1 - ��� ������������ (�� ����� ���� NULL)
  
  $2 - �������� ������ ������������ (�� ����� ���� NULL)
  
  $3 - �������� ������������ (�� ����� ���� NULL)
  
  $4 -  ������ ������������ (����� ���� NULL)
  
  $5 - ���� ������������ ���� �����, � ������� ������ ������ ������������ (���� TRUE, �� ������ �����������)
  
  Returns:
  ������������� ���������� ������������
  
  � ������, ���� �����-���� �� ������� ����������, ����� ������, ����� NULL ��� ��� �������� ������������ ��������� ������, �� ������������ ����������.
*/

CREATE OR REPLACE FUNCTION addUser(name, name, varchar, varchar, varchar, varchar, int4, name, boolean) RETURNS INTEGER AS 
$BODY$
DECLARE
    uname ALIAS FOR $1;
    gname ALIAS FOR $2;
    last_name ALIAS FOR $3;
    first_name alias for $4;
    sir_name alias for $5;
    the_fio alias for $6;
    idRank alias for $7;
    passwd ALIAS FOR $8;
    needInherit alias for $9;

    uid INTEGER;    
    gid INTEGER;
    query VARCHAR;
    created int4;
    res int4;
BEGIN
    
    if(uname isnull or gname isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;
    if(uname = 'postgres') then
	raise exception 'Username cannot be postgres!';
    end if;

--    select safeCreateUser(uname, passwd, false) into created;

--    if(created = 0) then
--	select count(usename) into res from pg_user where usename = uname;
--	if(res = 0) then 
--	    raise exception 'Cluster already contains USER with specified name. New USER must have another name!';
--	end if;
--    end if;

--    query := 'alter group ' || gname || ' add user ' || uname;
--    EXECUTE query;

    select getNextSeq('kks_roles', 'id') into uid;

    query := 'INSERT INTO Users (id, id_rank, role_name, lastname, firstname, sirname, fio, with_inheritance) VALUES (' ||
                                           uid || ',' ||
					   idRank  || ',' ||
                                           quote_literal(uname) || ',' ||
                                           quote_literal(last_name) || ',' ||
                                           quote_literal(first_name) || ',' ||
                                           quote_literal(sir_name) || ',' ||
                                           quote_literal(the_fio)  || ',' ||
                                           asString(needInherit, false) || ')';
    EXECUTE query;
        
    RETURN uid;
END;
$BODY$
LANGUAGE 'plpgsql';
