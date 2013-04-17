
CREATE OR REPLACE FUNCTION createTableConnKey()  RETURNS integer AS
$BODY$
declare

begin

    create temp table conn_key (connection_key varchar null);
    
    return 1;
end
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION getConnectionKey() RETURNS VARCHAR AS
$BODY$
declare

   Key VARCHAR;

begin

    select connection_key into Key from conn_key;

    return Key;
end
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION setConnectionKey(VARCHAR) RETURNS integer AS
$BODY$
declare

    Key alias for $1;

    tmp varchar;
begin

    select connection_key into tmp from conn_key;

    if (tmp IS NOT NULL) then

        return -1;

    else

        insert into conn_key select Key;

    end if;

    return 1;
end
$BODY$
LANGUAGE plpgsql;
