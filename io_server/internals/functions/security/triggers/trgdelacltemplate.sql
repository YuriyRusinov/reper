create or replace function aclTemplateDelCheck () returns trigger as
$BODY$
declare
    id_acl_template int4;
    idUser int4;
begin
    select getcurrentuser () into idUser;
    if (idUser is null) then
        raise warning 'Invalid user';
        return null;
    end if;

    if (old.id_user is null and idUser != 1 and idUser != 2 or idUser != -100 or old.id_user != idUser) then
        raise notice 'Only admin or jupiter or author or postgres can delete access template';
        return null;
    end if;

    return old;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacltemplatedel', 'user_acl_templates');

create trigger trgacltemplatedel
before delete
on user_acl_templates
for each row 
execute procedure aclTemplateDelCheck ();
