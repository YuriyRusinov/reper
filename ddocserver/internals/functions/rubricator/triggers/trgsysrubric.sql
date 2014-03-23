create or replace function isSysRubricatorUpdate () returns trigger as
$BODY$
declare
begin
    if (new.id = 1 and (new.name <> old.name or new.code <> old.code)) then
        raise warning 'Cannot update root rubricator';
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

create or replace function isSysRubricatorDelete () returns trigger as
$BODY$
declare
begin
    if (old.id = 1) then
        raise warning 'Cannot delete root rubricator';
        return NULL;
    end if;

    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgsysrubricatorupdate', 'rubricator');
create trigger trgSysRubricatorUpdate
before update
on rubricator
for each row
execute procedure isSysRubricatorUpdate();

select f_safe_drop_trigger('trgsysrubricatordelete', 'rubricator');
create trigger trgSysRubricatorDelete
before delete
on rubricator
for each row
execute procedure isSysRubricatorDelete();
