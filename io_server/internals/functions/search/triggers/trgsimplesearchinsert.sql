create or replace function simpleSearchInsertCheck() returns trigger as
$BODY$
declare
    ok bool;
    ioType int4;
    idGroup int4;
    idTemplate int4;
    idCriterion int4;

    idAGroup int4;
    idOldAGroup int4;

    idAttr int4;
    idOperation int4;
    theValue varchar;

    r record;
begin

    if(new.is_completed = 0) then
        return new;
    end if;

    ok = false;

    if(old.is_completed = 0 and (new.is_completed = 1 or new.is_completed = 2 )) then
        ok = true;
    end if;

    if(ok <> true) then
        return new;
    end if;


    select id_io_category_type into ioType from io_categories where id = new.id_io_category;
    if(ioType <> 12) then
        return new;
    end if;

    --в БД создан ИО, который является шаблоном поискового запроса
    --теперь необходимо перенести всю информацию в таблицы, хранящие данные о поисковых запросах
    
    select getNextSeq('groups', 'id') into idGroup;
    insert into groups (id, id_parent, operation, is_not) values(idGroup, NULL, 1, false);

    select getNextSeq('search_templates', 'id') into idTemplate;
    insert into search_templates (id, id_group, author, name, description) values(idTemplate, idGroup, new.author, new.name, new.description);
    
    idAGroup = -1;
    idOldAGroup = -1;

    idAttr = 0;
    idOperation = 0;
    theValue = '';
    
    for r in 
        select distinct
            ac.id_io_attribute,
            a.code,
            av.value,
            v.id_a_group
        from 
            attrs_values av,
            attrs_categories ac,
            io_categories c,
            io_templates t,
            io_views v,
            attributes a
        where 
            av.id_io_object = 445
            av.id_attr_category = ac.id
            and ac.id_io_category = c.id
            and ac.id_io_attribute = a.id
            and c.id = t.id_io_category
            and t.id = v.id_io_template
            and v.id_attr_category = ac.id
        order by 4
    loop

        idAGroup = r.id_a_group;

        if(idOldAGroup <> -1 and idOldAGroup <> idAGroup) then --обрабатываем новую группу атрибутов

            if(idOperation = 0) then
                idOperation = 1;
            end if;
            
            if(idAttr > 0 and theValue is not null) then
                select getNextSeq('criteria', 'id') into idCriterion;
                insert into criteria (id, attr_id, value, id_type, id_operation, case_sensitive, is_not)
                values (idCriterion, idAttr, theValue, 6, idOperation, false, false);

                insert into groups_criteria (id_group, id_criteria) values(idGroup, idCriterion);
            end if;

            idOperation = 0;
            idAttr = 0;
            theValue = '';
        end if;

        if(substring(r.code from 1 for 16) = 'OPERATIONS_ATTR_') then
            idOperation = r.value;
        else 
            idAttr = r.id_io_attribute;
            theValue = r.value;
        end if;

        idOldAGroup = idAGroup;
    
    end loop;


    if(idAttr > 0 and theValue is not null) then
        select getNextSeq('criteria', 'id') into idCriterion;
        insert into criteria (id, attr_id, value, id_type, id_operation, case_sensitive, is_not)
        values (idCriterion, idAttr, theValue, 6, idOperation, false, false);

        insert into groups_criteria (id_group, id_criteria) values(idGroup, idCriterion);
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsimplesearchinsert', 'tbl_io_objects');

create trigger trgSimpleSearchInsert 
before update
on tbl_io_objects
for each row 
execute procedure simpleSearchInsertCheck();

