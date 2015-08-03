/*
  Function: getUser(int4)
  
  Функция возвращает имя пользователя по его идентификатору в БГД
*/
CREATE OR REPLACE FUNCTION getUser(int4) RETURNS VARCHAR AS 
$BODY$
DECLARE
    uid ALIAS FOR $1;
    uname VARCHAR;
BEGIN
    SELECT role_name INTO uname FROM users WHERE id = uid;
    RETURN uname;
END;
$BODY$
LANGUAGE 'plpgsql';

/*
  Function: getUser(varchar)
  
  Функция возвращает идентификатор пользователя в БГД по его имени.
*/
CREATE OR REPLACE FUNCTION getUser(VARCHAR) RETURNS INTEGER AS 
$BODY$
DECLARE
    uname ALIAS FOR $1;
    uid INTEGER;
BEGIN
    SELECT id INTO uid FROM users WHERE role_name = uname;
    RETURN uid;
END;
$BODY$
LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION getCurrentUser() RETURNS INTEGER AS 
$BODY$
DECLARE
    userName varchar;
    uid INTEGER;
BEGIN

    select session_user into userName;
    if(userName = 'postgres') then
        return -100;
    end if;

    SELECT id INTO uid FROM users WHERE role_name = userName;
    if(uid isnull) then
        return -1;
    end if;

    RETURN uid;
END;
$BODY$
LANGUAGE 'plpgsql';
