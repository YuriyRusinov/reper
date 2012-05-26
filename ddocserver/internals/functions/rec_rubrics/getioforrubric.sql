--������ ������� ���������� � ������� ��������������, ��������� ������ ��� ��������� ����������, � ������ �� ��������� ������ ������� (���� ���������)
--������������ ��� �������� ���� �������
--��������� ������ ���������� ���� ���������������� ������������� �� ������������ ������ � ����� ������ ���������� ������������� �������
create or replace function getIOForRubric(int4) returns int4 as
$BODY$
declare
    idRubric alias for $1;
    idObject int4;
    idParent int4;
    r record;
begin

    for r in
        select id_io_object, id_parent from rubricator where id = idRubric
    loop
        idObject = r.id_io_object;
        idParent = r.id_parent;
    end loop;
    
    if(idParent isnull) then
        return idObject;
    end if;

    if(idObject isnull) then
        select getIOForRubric(idParent) into idObject from rubricator where id = idRubric;
    end if;

    return idObject;

end
$BODY$
language 'plpgsql' security definer;
