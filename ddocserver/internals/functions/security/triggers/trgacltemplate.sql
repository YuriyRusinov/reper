create or replace function aclTemplateCheck () returns trigger as
$BODY$
declare
    id_acl_template int4;
    idUser int4;
begin
    select getcurrentuser () into idUser;

    if (idUser is null) then
        raise warning 'Invalid user';
        return NULL;
    end if;

    if (new.id_user is null and idUser != 1 and idUser != 2 and idUser != -100) then
        raise warning 'Only admin or jupiter or postgres can insert/update system template';
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacltemplate', 'user_acl_templates');

create trigger trgacltemplate
before insert or update
on user_acl_templates
for each row 
execute procedure aclTemplateCheck ();
