select f_safe_drop_type('h_get_oshs');
create type h_get_oshs as(id int4, 
                          id_parent int4, 
                          name varchar, 
                          with_inheritance bool, 
                          type int4); 


create or replace function getOrganizations(int4) returns setof h_get_oshs as 
$BODY$
declare 
    idParent alias for $1;
    

    rec h_get_oshs%rowtype;
    rr h_get_oshs%rowtype;
begin

    if(idParent isnull) then
        for rec in
            select e.id, e.id_parent, e.name, NULL, 0
            from organization e
            where 
                e.id_parent is null order by e.id
        loop
            return next rec;

            for rr in 
                select * from getUnits(rec.id, NULL)
            loop
                return next rr;
            end loop;

            for rr in 
                select * from getOrganizations(rec.id)
            loop
                return next rr;
            end loop;

        end loop;
    else
        for rec in
            select e.id, e.id_parent, e.name, NULL, 0
            from organization e
            where 
                e.id_parent = idParent
        loop
            return next rec;

            for rr in 
                select * from getUnits(rec.id, NULL)
            loop
                return next rr;
            end loop;

            for rr in 
                select * from getOrganizations(rec.id)
            loop
                return next rr;
            end loop;

        end loop;
    end if;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function getUnits(int4, int4) returns setof h_get_oshs as
$BODY$
declare 
    idOrganization alias for $1;
    idParent alias for $2;
    
    rec h_get_oshs%rowtype;
    rr h_get_oshs%rowtype;
    idLocalOrg int4;
begin

    idLocalOrg = getLocalOrgId();
    
    if(idParent isnull) then
        for rec in
            select u.id, u.id_parent, u.name, u.with_inheritance, 1
            from units u, organization o
            where 
                u.id_organization = idOrganization
                and o.id = u.id_organization
                and u.id_parent is null
                and ((o.id = idLocalOrg and u.name <> 'Публичные должностные лица') or o.id <> idLocalOrg)   
        loop
            return next rec;

            for rr in
                select * from getDls(rec.id)
            loop
                return next rr;
            end loop;

            for rr in
                select * from getUnits(idOrganization, rec.id)
            loop
                return next rr;
            end loop;

        end loop;
    else
        for rec in
            select u.id, u.id_parent, u.name, u.with_inheritance, 1
            from units u, organization o
            where 
                u.id_organization = idOrganization
                and o.id = u.id_organization
                and u.id_parent = idParent
                and ((o.id = idLocalOrg and u.name <> 'Публичные должностные лица') or o.id <> idLocalOrg)   
        loop
            return next rec;

            for rr in 
                select * from getDls(rec.id)
            loop
                return next rr;
            end loop;

            for rr in 
                select * from getUnits(idOrganization, rec.id)
            loop
                return next rr;
            end loop;

        end loop;

    end if;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function getDls(int4) returns setof h_get_oshs as
$BODY$
declare
    idUnit alias for $1;
    
    rec h_get_oshs%rowtype;
    rr h_get_oshs%rowtype;
begin

    for rec in 
        select d.id, NULL, d.name, d.with_inheritance, 2
        from "position" d
        where d.id_unit = idUnit
        order by 3
    loop

        return next rec;

        for rr in 
            select * from getUserDl(rec.id)
        loop
            return next rr;
        end loop;
        
    end loop;

    return;    
end
$BODY$
language 'plpgsql';

create or replace function getUserDl(int4) returns setof h_get_oshs as
$BODY$
declare
    idDl alias for $1;
    r h_get_oshs%rowtype;
begin

    for r in 
        select u.id, NULL, u.fio, u.with_inheritance, 3
        from users u, "position" d
        where 
	    d.id_user = u.id
            and d.id = idDl
        order by 1
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getDls () returns setof h_get_oshs as
$BODY$
declare
    r h_get_oshs%rowtype;
begin

    for r in 
        select d.id, NULL, d.name, d.with_inheritance, 2
        from "position" d
        order by 3
    loop
        return next r;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';
