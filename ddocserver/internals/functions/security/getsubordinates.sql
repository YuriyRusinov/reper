/*возвращает список должностных лиц, €вл€ющихс€ подчиненными данного лица*/
select f_safe_drop_type('h_get_subordinates');
create type h_get_subordinates as (id int4,
                                   id_unit int4,
                                   id_maclabel int4,
                                   id_user int4,
                                   user_name varchar,
                                   name varchar,
                                   short_name varchar,
                                   is_io bool);

create or replace function getSubordinates(int4) returns setof h_get_subordinates as
$BODY$
declare
    idBoss alias for $1;
    r record;
begin

    for r in
        select 
            p.id,
            p.id_unit,
            p.id_maclabel,
            p.id_user,
            u.fio,
            p.name,
            p.short_name,
            p.is_io
        from
            "position" p,
            users u,
            units un
        where
            un.id_boss = idBoss
            and un.id = p.id_unit
            and p.id_user = u.id
            and p.id_user_vrio isnull
            and p.id_user is not null

        union
            
        select 
            p.id,
            p.id_unit,
            p.id_maclabel,
            p.id_user_vrio,
            u.fio,
            p.name,
            p.short_name,
            p.is_io
        from
            "position" p,
            users u,
            units un
        where
            un.id_boss = idBoss
            and un.id = p.id_unit
            and p.id_user_vrio = u.id
            and p.id_user_vrio is not null
            and p.id_user isnull
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';
