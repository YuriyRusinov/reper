select f_safe_drop_type('h_unit_desc');
create type h_unit_desc as(id_unit int4,
                           unit_name varchar
                           );

create or replace function getMyUnit() returns setof h_unit_desc as
$BODY$
declare
    r h_unit_desc%rowtype;
begin
    for r in 
        select u.id, u.name
        from units u, "position" p
        where
            p.id = getCurrentDl()
            and p.id_unit = u.id
    loop
        return next r;
    end loop;

    return;
end 
$BODY$
language 'plpgsql';

create or replace function getUnitList(int4) returns setof h_unit_desc as
$BODY$
declare
    idUser alias for $1;
    r h_unit_desc%rowtype;
begin
    for r in 
        select u.id, u.name
        from units u, "position" p
        where
            p.id_user = idUser 
            and p.id_unit = u.id
    loop
        return next r;
    end loop;

    return;
end 
$BODY$
language 'plpgsql';
