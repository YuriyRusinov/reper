create or replace function getRecordTable(int8) returns varchar as
$BODY$
declare
    idRecord alias for $1;

    tName1 varchar;
    tName2 varchar;


    idObject1 int4;
    idObject2 int4;

    pos int4;
begin

    SELECT p.relname into tName1
    FROM q_base_table q, pg_class p
    WHERE q.id = idRecord AND q.tableoid = p.oid;
    
    --� ���� ����, ��� �� "���������" ������� ��������������� � ������ �������� ������������� ���������� � ���� table_name ������� tbl_io_objects,
    --��� ���������� ������� ������ �� �������� (������� ��� �������������), ������� �������� � tbl_io_objects
    --������� �� ������ ���������������� ���������� � tName1 ��������

    pos = position('tbl_' in tName1);
    if(pos == 1) then --���� �������� ���������� ������� ���������� � tbl_ , �� ����� ������������, ��� � io_objects 
        tName2 = substring(tName1 from 5);
        if(char_length(tName2) = 0) then
            return tName1;
        end if;
    else --� ���� ������ � Io_objects �������� �������� ���������� ������� (acl_secureTable() ��� ��� �� ����������)
        return tName1;
    end if;

    
    select id into idObject1 from tbl_io_objects where table_name = tName1; --������ ��� ���������� �������. �� � tbl_io_objects ����� ���� �������� ����, ������� ������ ������ ������
    select id into idObject2 from tbl_io_objects where table_name = tName2; --���������������� ������ ��� �������� � tbl_io_objects

    if(idObject1 is not null and idObject2 is not null) then
        return tName1;
    end if;

    if(idObject2 is not null) then
        return tName2;
    end if;

    return tName1;
end
$BODY$
language 'plpgsql' security definer;
