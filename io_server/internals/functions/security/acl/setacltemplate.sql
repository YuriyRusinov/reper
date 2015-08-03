create or replace function setAclTemplate (varchar) returns int4 as
$BODY$
declare
    template_name alias for $1;
    idUser int4;
    idt int4;
begin
    select getcurrentuser () into idUser;
    if (idUser is null) then
        return -1;
    end if;
    select getNextSeq ('user_acl_templates', 'id') into idt;
    raise notice 'id is %', idt;

    insert into user_acl_templates (id, id_user, name) values (idt, idUser, template_name);
    if (not FOUND) then
        return -1;
    end if;

    return idt;
end
$BODY$
language 'plpgsql';

create or replace function setAclTemplate (int4, varchar) returns int4 as
$BODY$
declare
    idt alias for $1;
    template_name alias for $2;
    idUser int4;
begin
    select getcurrentuser () into idUser;
    if (idUser is null) then
        return -1;
    end if;

    update user_acl_templates set name = template_name where id=idt;
    if (not FOUND) then
        return -1;
    end if;

    return idt;
end
$BODY$
language 'plpgsql';

create or replace function delAclTemplate (int4) returns int4 as
$BODY$
declare
    idt alias for $1;
    idUser int4;
begin
    select getcurrentuser () into idUser;
    if (idUser is null) then
        return -1;
    end if;

    delete from acl_template_groups where id_template = idt;
    delete from acl_template_exceptions where id_template = idt;
    delete from user_acl_templates where id = idt;
    if (not FOUND) then
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function setAclTemplateParams (int4, int4, bool, bool, bool, bool, bool, bool) returns int4 as
$BODY$
declare
    idTemplate alias for $1;
    idRole alias for $2;

    aReadList alias for $3;
    aRead alias for $4;
    aUpdate alias for $5;
    aDelete alias for $6;
    aSend alias for $7;

    bIsSystem alias for $8;

    cnt int4;

begin

    if(bIsSystem isnull or not bIsSystem) then

        select count(*) into cnt from acl_template_exceptions where id_role = idRole and id_template = idTemplate;
        if(cnt > 0) then
            update acl_template_exceptions set allow_readlist = aReadList, allow_read = aRead, allow_delete = aDelete, allow_update = aUpdate, allow_use1 = aSend;
--            update ...  where id_role = idRole and id_template = idTemplate;
        else

            insert into acl_template_exceptions (id_template, id_role, allow_read, allow_readlist, allow_delete, allow_update, allow_use1)
                                        values  (idTemplate, idRole, aRead, aReadList, aDelete, aUpdate, aSend);
        end if;
    else
        select count(*) into cnt from acl_template_groups where id_group_type = idRole and id_template = idTemplate;
        if(cnt > 0) then
            update acl_template_groups set allow_readlist = aReadList, allow_read = aRead, allow_delete = aDelete, allow_update = aUpdate, allow_use1 = aSend where id_group_type = idRole and id_template = idTemplate;
--            update ...  where id_role = idRole and id_template = idTemplate;
        else
            insert into acl_template_groups     (id_template, id_group_type, allow_read, allow_readlist, allow_delete, allow_update, allow_use1)
                                        values  (idTemplate, idRole, aRead, aReadList, aDelete, aUpdate, aSend);
        end if;
    end if;

   return 1;

end
$BODY$
language 'plpgsql';

