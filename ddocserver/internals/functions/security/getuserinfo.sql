/*
  Function: getUserInfo(int4)
  
*/
select f_safe_drop_type('h_get_user_info');
create type h_get_user_info as (id int4,
                                id_rank int4,
                                rank_name varchar,
                                id_state int4,
                                state_name varchar,
                                id_maclabel int4,
                                maclabel_name varchar,
                                lastname varchar,
                                firstname varchar,
                                sirname varchar,
                                fio varchar,
                                insert_time timestamp,
                                email varchar,
                                login_name varchar,
                                is_connected bool);

CREATE OR REPLACE FUNCTION getUserInfo(int4) RETURNS setof h_get_user_info AS 
$BODY$
DECLARE
    idUser ALIAS FOR $1;

    r h_get_user_info%rowtype;
BEGIN
    for r in 
        select u.id,
               u.id_rank,
               rr.name,
               u.id_state,
               s.name,
               u.id_maclabel,
               m.name,
               u.lastname,
               u.firstname,
               u.sirname,
               u.fio,
               u.insert_time,
               u.email,
               u.role_name,
               u.is_connected
        from
             users u
        left join ranks rr on (rr.id = u.id_rank)
        left join user_state s on (s.id = u.id_state)
        left join maclabels m on (m.id = u.id_maclabel)

        where u.id = idUser
    loop
        return next r;
    end loop;


    return;

END;
$BODY$
LANGUAGE 'plpgsql';



CREATE OR REPLACE FUNCTION getCurrentUserInfo() RETURNS setof h_get_user_info AS 
$BODY$
DECLARE
    userName varchar;
    uid INTEGER;
    r h_get_user_info%rowtype;
BEGIN

    select session_user into userName;
    if(userName = 'postgres') then
        return;
    end if;

    SELECT id INTO uid FROM users WHERE role_name = userName;
    if(uid isnull) then
        return;
    end if;

    for r in select * from getUserInfo(uid)
    loop
        return next r;
    end loop;

    RETURN;
END;
$BODY$
LANGUAGE 'plpgsql';
