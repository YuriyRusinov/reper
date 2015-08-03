create or replace function aclPrivilegiesCheck () returns trigger as
$BODY$
declare
    idUser int4;
    idAclTemplate int4;
    isSystem boolean;
    query varchar;

    r RECORD;
begin
    select getcurrentuser () into idUser;
    if (idUser is null) then
        raise warning 'Invalid user';
        return NULL;
    end if;

    query := 'select uat.id_user is null as is_system from ' || TG_TABLE_NAME || ' te inner join user_acl_templates uat on (te.id_template = uat.id and te.id_template=' || new.id_template || ');';
    execute query into isSystem;
    --select into isSystem uat.id_user is null as is_system from TG_TABLE_NAME te inner join user_acl_templates uat on (te.id_template = uat.id and te.id_template=new.id_template);
    if (isSystem and idUser != 1 and idUser !=2 and idUser != -100) then
        raise warning 'Only admin or jupiter or postgres can update/delete system templates';
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgacltemplategroup', 'acl_template_groups');

create trigger trgacltemplategroup
after insert or update
on acl_template_groups
for each row 
execute procedure aclPrivilegiesCheck ();

select f_safe_drop_trigger('trgacltemplategroup', 'acl_template_exceptions');

create trigger trgacltemplategroup
after insert or update
on acl_template_exceptions
for each row 
execute procedure aclPrivilegiesCheck ();
