create or replace function hStartHandler(varchar, int4) returns int4 as
$BODY$
declare
    iService alias for $1;
    iId alias for $2;
    a int4;
begin

    select f_is_table_exist('aaaaa', 'public') into a;
    if(a = 0) then
        create table aaaaa (service varchar, id int4);
    end if;

    insert into aaaaa (service, id) values (iService, iId);

    return 1;

end
$BODY$
language 'plpgsql';
