select f_safe_drop_type('h_gud_get_user_dls');
create type h_gud_get_user_dls as(id int4, 
                                  id_unit int4, 
                                  id_user int4, 
                                  is_io boolean, 
                                  dls_name varchar, 
                                  unit_name varchar);

create or replace function getUserDls() returns setof h_gud_get_user_dls as
$BODY$
declare
    r h_gud_get_user_dls%rowtype;
    e_cnt int4;
begin

    for r in select * from getUserDls(getCurrentUser())
    loop
        return next r;
    end loop;

    return;
  
end
$BODY$
language 'plpgsql';

create or replace function getUserDls(int4) returns setof h_gud_get_user_dls as
$BODY$
declare
    idUser alias for $1;
--    e_cnt int4;

    r h_gud_get_user_dls%rowtype;
begin

--    select count(*) into e_cnt from organization;

/*
    if (e_cnt = 0) then
        for r in 
          select
            p.id,
            p.id_unit,
            p.id_user,
            p.is_io,
            p.name,
            u.name
          from 
            "position" p,
            units u
          where 
            p.id_user = idUser
            and p.id_unit = u.id
        loop
            return next r;
        end loop;
    else
*/
        for r in 
          select distinct 
            p.id,
            p.id_unit,
            p.id_user,
            p.is_io,
            p.name,
            u.name
          from 
            "position" p,
            units u
          where 
            p.id_user = idUser
            and p.id_unit = u.id
            and (isLocalDl(p.id) = TRUE) 
        loop
            return next r;
        end loop;
--    end if;

    return;
   
end
$BODY$
language 'plpgsql';
