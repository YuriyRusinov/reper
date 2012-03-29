CREATE OR REPLACE FUNCTION userInsertCheck() RETURNS trigger AS
$BODY$
DECLARE

    uname varchar;    
    passwd varchar;
    created int4;
    res int4;
BEGIN


    uname = new.role_name;
    passwd = new.role_name;
    
    if(uname isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;

    if(uname = 'postgres') then
	raise exception 'Username cannot be postgres!';
    end if;

    if(uname = 'admin') then
	--raise notice 'Username cannot be admin!';
        return new;
    end if;


    select safeCreateUser(uname, passwd, false) into created;

    if(created = 0) then
	select count(usename) into res from pg_user where usename = uname;
	if(res = 0) then 
	    raise exception 'Cluster already contains USER with specified name. New USER must have another name!';
	end if;
    end if;

    RETURN new;
END;
$BODY$
LANGUAGE 'plpgsql';

select f_safe_drop_trigger('trguserinsert', 'users');

create trigger trgUserInsert
after insert 
on users 
for each row 
execute procedure userInsertCheck();
