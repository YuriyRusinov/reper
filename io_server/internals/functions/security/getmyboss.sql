select f_safe_drop_type('h_boss_desc');
create type h_boss_desc as(id_dl int4,
                           id_user int4,
                           dl_name varchar,
                           user_fio varchar,
                           user_lastname varchar,
                           user_firstname varchar,
                           user_sirname varchar
                           );
                           
create or replace function getMyBoss() returns setof h_boss_desc as
$BODY$
declare
    r h_boss_desc%rowtype;
begin

    for r in
        select 
            p.id, 
            u.id, 
            p.name, 
            u.fio, 
            u.lastname, 
            u.firstname, 
            u.sirname
        from "position" p, users u, units un
        where
            p.id_user = u.id
            and p.id = un.id_boss
            and un.id = (select id_unit from getMyUnit())
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getBossList(int4) returns setof h_boss_desc as
$BODY$
declare
    idUser alias for $1;
    r h_boss_desc%rowtype;
begin

    for r in
        select 
            p.id, 
            u.id, 
            p.name, 
            u.fio, 
            u.lastname, 
            u.firstname, 
            u.sirname
        from "position" p, users u, units un
        where
            p.id_user = u.id
            and p.id = un.id_boss
            and p.id_unit in (select id_unit from getUnitList(idUser))
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
