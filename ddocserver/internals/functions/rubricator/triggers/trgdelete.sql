create or replace function userRubricatorDelete() returns trigger as
$BODY$
declare
    idUser int4;
begin
    idUser = getCurrentUser();

    if(idUser = 1 or idUser = 2) then
        return old;
    end if;

    if(old.id_user <> idUser) then
        raise exception 'You cannot delete not yours rubricators';
        return NULL;
    end if;

    return old;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trguserrubricatordelete', 'user_rubricator');

create trigger trgUserRubricatorDelete
before delete
on user_rubricator
for each row 
execute procedure userRubricatorDelete();
