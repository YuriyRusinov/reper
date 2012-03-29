select f_safe_drop_type('h_get_acl_templates');
create type h_get_acl_templates as (id int4,
                                    name varchar,
                                    is_system bool);

create or replace function getAclTemplates () returns setof h_get_acl_templates as
$BODY$
declare
    idUser int4;
    rr h_get_acl_templates%rowtype;
begin

    select getcurrentuser () into idUser;
    if (idUser is null) then
        return;
    end if;

    for rr in
        select id,name, (uat.id_user is null) as is_system from user_acl_templates as uat where uat.id_user = idUser or uat.id_user is null
    loop
        return next rr;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;

create or replace function getAclTemplate (int4) returns setof h_get_acl_templates as
$BODY$
declare
    idTemplate alias for $1;
    idUser int4;
    rr h_get_acl_templates%rowtype;
begin

    if (idTemplate is null or idTemplate <= 0) then
        return;
    end if;

    select getcurrentuser () into idUser;
    if (idUser is null) then
        return;
    end if;

    for rr in
        select id,name, (uat.id_user is null) as is_system from user_acl_templates as uat where ((uat.id_user=idUser or uat.id_user is null) and uat.id = idTemplate)
    loop
        return next rr;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;

create or replace function clearAclTemplate (int4, int4) returns int4 as 
$BODY$
declare
    idUser alias for $1;
    idAclTemplate alias for $2;
begin

    delete from user_acl_templates where id_user = idUser and id = idAclTemplate;

    if (FOUND = FALSE) then
        raise notice 'There are no user acl templates found!';
    end if;

    return 1;
end 
$BODY$
language 'plpgsql' security definer;
