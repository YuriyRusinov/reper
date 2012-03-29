create or replace function ioInsertOrgs(int4, int4[]) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idOrgs alias for $2;

    insertQuery varchar;
    v int4;
    i int4;
    c int4;
    cnt int4;
begin

    cnt := array_upper(idOrgs, 1);

    
    if(cnt isnull or cnt = 0) then
        delete from io_objects_organization where id_io_objects = idObject;
        return 1;
    end if;

    --delete records that must be deleted
    delete from io_objects_organization 
    where 
        id_io_objects = idObject 
        and id_organization in  (
                                 select id_organization 
                                 from io_objects_organization 
                                 where 
                                     id_io_objects = idObject 
                                      and id_organization not in 
                                                           (select id_organization 
                                                            from io_objects_organization 
                                                            where 
                                                                id_io_objects = idObject 
                                                                and id_organization = ANY (idOrgs)));

    
    insertQuery := ' ';
    for i in 1..cnt
    loop
        v := idOrgs[i];
        select count(*) into c from io_objects_organization where id_io_objects = idObject and id_organization = v;
        if(c = 0) then
            insertQuery := insertQuery || ' insert into io_objects_organization (id_io_objects, id_organization) values (' || idObject || ', ' || v || ');';
        end if;

    end loop;

    raise notice '%', insertQuery;

    execute insertQuery;

    return 1;
end
$BODY$
language 'plpgsql';
