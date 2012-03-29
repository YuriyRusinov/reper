create or replace function iQualifiersUpdate() returns trigger as
$BODY$
declare
    the_uid varchar;
    orgName varchar;
    q varchar;
    cExist bool;
begin
    if(not (old.is_created <> 1 and new.is_created = 1)) then
        return new;
    end if;

    select org_name into orgName from getLocalOrg();
    if(orgName isnull) then
        --raise exception 'iQualifierUpdate: ERROR! organization name is NULL!';
        return new;
    end if;

    --just verification
    the_uid = generateUID(66, 'io_objects');
    if(the_uid = 'localorg-io_objects-66') then
        raise exception 'iQualifiersUpdate: ERROR! UID is still localorg!';
        return NULL;
    end if;

    cExist = f_is_column_exist('access_cards_q', 'access_plans');
    if(cExist = FALSE) then
        return new;
    end if;

    update attributes 
    set 
        unique_id = generateUID(id, 'attributes'),
        code = generateUID(id, 'attributes'),
        name = name || ' (' || orgName || ')'
    where
        id in (181, 186); --access_plans and access_cards (поскольку они являются ссылками)

    --поскольку коды атрибутов являются названиями колонок в таблицах, 
    --то необходимо изменить эти колонки
    q = 'alter TABLE access_cards_q RENAME COLUMN access_plans to "' || generateUID(186, 'attributes') || '"';
    execute q;
    q = 'alter TABLE personal_q RENAME COLUMN access_cards to "' || generateUID(181, 'attributes') || '"';
    execute q;



    update io_categories
    set 
        unique_id = generateUID(id, 'io_categories'),
        name = name || ' (' || orgName || ')'
    where
        id in (101, 102, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142);


    update io_objects 
    set 
        unique_id = generateUID(id, 'io_objects'),
        is_system = false,
        name = name || ' (' || orgName || ')'
    where id in(50, 66, 67, 68, 69, 70); --IK TSO

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('zz_trgqualifiersupdate', 'organization');

select f_create_trigger('zz_trgqualifiersupdate', 'after', 'update', 'organization', 'iqualifiersupdate()');

/*
create trigger zz_trgQualifiersUpdate
after update
on organization
for each row
execute procedure iQualifiersUpdate();
*/
