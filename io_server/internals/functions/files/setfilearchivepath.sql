create or replace function setFileArchivePath(varchar) returns int4 as
$BODY$
declare
    fPath alias for $1;
    cnt int4;
begin

    select count(*) into cnt from system_table where id_system = 1;

    if(cnt = 0) then
        insert into system_table (id_system, str_parameter, description) values (1, fPath, 'file archive path');
        if(FOUND = FALSE) then
            raise notice 'Cannot insert file archive path into system_table!';
            return 0;
        end if;
    else
        update system_table set str_parameter = fPath, description = 'file archive path' where id_system = 1;
        if(FOUND = FALSE) then
            raise notice 'Cannot update file archive path in system_table!';
            return 0;
        end if;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';
