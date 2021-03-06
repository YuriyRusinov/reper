create or replace function cDeleteCategory(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
    r record;
begin

    perform cDeleteRubrics(idCategory);
    update rubricator set id_io_category = NULL where id_io_category = idCategory;--�������, ������� ���������� ��������� ���������� �� ������������
    delete from access_categories_table where id_io_category = idCategory;

    delete from object_ref_tables where id_io_category = idCategory;    --����� ����� ��������� ������� (���������� ��������������� �������)

    --���������� ��������� ������ � ��������� ���������������� �������� � ������������
    --������ �� ��� ��������� ��� ����������?
    delete from user_handlers_data where id_io_category = idCategory;
    delete from user_chains_organization where id_user_chains in (select id from user_chains where id_io_category = idCategory);
    delete from user_chains where id_io_category = idCategory;
    
/*
    delete from tsd_control_journal where id_journal in (select id from command_journal where id_io_category = idCategory);
    delete from cmd_confirmations where id_cmd in (select id from command_journal where id_io_category = idCategory);
    delete from command_journal where id_io_category = idCategory;

    delete from tsd_control_journal where id_journal in(select id from command_journal where id_tabel in (select id from tsd where id_io_category = idCategory));    
    delete from cmd_confirmations where id_cmd in (select id from command_journal where id_tabel in (select id from tsd where id_io_category = idCategory));
    delete from command_journal where id_tabel in (select id from tsd where id_io_category = idCategory);
    delete from tsd where id_io_category = idCategory;
*/

    perform tDeleteTemplate(id) from io_templates where id_io_category = idCategory;

    --delete from tbl_attrs_values where id_attr_category in (select id from attrs_categories where id_io_category = idCategory);
    --delete from attrs_categories where id_io_category = idCategory;
    --delete from io_categories where id = idCategory;
    
    update io_categories set is_archived = true where id = idCategory;

    for r in
        select id_child from io_categories where id = idCategory
    loop
        raise warning '%', r.id_child;
        if(r.id_child is not null) then
            perform cDeleteCategory(r.id_child);-- from io_categories where id = idCategory and id_child is not null;
        end if;
    end loop;
    for r in
        select id_child2 from io_categories where id = idCategory
    loop
        raise warning '%', r.id_child2;
        if(r.id_child2 is not null) then
            perform cDeleteCategory(r.id_child2);-- from io_categories where id = idCategory and id_child is not null;
        end if;
    end loop;

    return 1;

end
$BODY$
language 'plpgsql' security definer;
