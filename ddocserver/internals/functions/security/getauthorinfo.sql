create or replace function getAuthorInfo() returns varchar as
$BODY$
declare
    info varchar;
    r record;
begin

    info = NULL;
    
    if(getCurrentDl() = 4) then --admin
        info = 'Администратор системы';
        return info;
    end if;

    for r in 
        select 
            u.fio, 
            p.name as p_name, 
            p.phone, 
            un.name as un_name, 
            (case when un.id_organization is not null then o.name else NULL end) as o_name
        from users u, "position" p, units un, organization o
        where 
            (u.id = p.id_user or u.id = p.id_user_vrio)
            and p.id_unit = un.id
            and p.id = getCurrentDl()
            and (un.id_organization isnull or un.id_organization = o.id)
    loop
        info = r.fio;
        info = info || E'\n';
        if(r.phone is not null) then
            info = info || 'Телефон: ' || r.phone;
            info = info || E'\n';
        end if;

        if(r.o_name is not null) then
            info = info || r.o_name;
            info = info || E'\n';
        end if;

        info = info || r.un_name;
        info = info || E'\n';
        info = info || r.p_name;
    end loop;       

    if(FOUND = FALSE or info isnull) then
        info = 'Нет информации об авторе';
    end if;

    return info;
end
$BODY$
language 'plpgsql';
