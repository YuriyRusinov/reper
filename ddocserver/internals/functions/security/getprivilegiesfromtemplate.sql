select f_safe_drop_type('h_get_privilegies_template');
create type h_get_privilegies_template as (id_role int4,
                                           id_template int4,
                                           allow_readlist boolean,
                                           allow_read boolean,
                                           allow_update boolean,
                                           allow_delete boolean,
                                           allow_send boolean,
                                           p_type int4,
                                           was_found boolean
                                           );

create or replace function getPrivilegiesFromTemplate (int4) returns setof h_get_privilegies_template as
$BODY$
declare
    idTemplate alias for $1;

    r h_get_privilegies_template%rowtype;
    cnt int4;
    query varchar;
    idUser int4;
begin

    select getcurrentuser() into idUser;
    select count(*) into cnt from acl_template_exceptions where id_template=idTemplate and id_role=idUser;
    query := 'select id_group_type, id_template, allow_readlist, allow_read, allow_update, allow_delete, allow_use1, 1 as type, true from acl_template_groups where id_template=' || idTemplate || ' union all select id_role, id_template, allow_readlist, allow_read, allow_update, allow_delete, allow_use1, 2 as type, true from acl_template_exceptions where id_template=' || idTemplate;
    if (cnt = 0) then
        query := query || 'union all select ' || idUser || ',' || idTemplate || ', true, true, true, true, true, 2 as type, false';
    end if;
    for r in
        execute query 
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
