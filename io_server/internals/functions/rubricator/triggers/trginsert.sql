create or replace function userRubricatorInsert() returns trigger as
$BODY$
declare
begin
    if(new.id_user isnull or new.id_user <> getCurrentUser()) then
        new.id_user = getCurrentUser();
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trguserrubricatorinsert', 'user_rubricator');
create trigger trgUserRubricatorInsert
before insert
on user_rubricator
for each row
execute procedure userRubricatorInsert();

