create or replace function userRubricatorUpdate() returns trigger as
$BODY$
declare
begin
    if(old.id_user <> getCurrentUser()) then
        raise exception 'You cannot modify not yours rubricators!';
        return NULL;
    end if;

    if(new.id_user isnull or new.id_user <> getCurrentUser()) then
        new.id_user = getCurrentUser();
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trguserrubricatorupdate', 'user_rubricator');
create trigger trgUserRubricatorUpdate
before update
on user_rubricator
for each row
execute procedure userRubricatorUpdate();

