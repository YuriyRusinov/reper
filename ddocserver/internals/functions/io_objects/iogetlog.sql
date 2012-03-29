select f_safe_drop_type('h_io_get_log');
create type h_io_get_log as(id int4, 
                            id_user int4,
                            user_name varchar, 
                            oper_type int4, 
                            datetime timestamp);

create or replace function ioGetLog(int4) returns setof h_io_get_log as
$BODY$
declare
    r h_io_get_log%rowtype;
    idObject alias for $1;
begin

    for r in
        select l.id, l.id_user, u.fio, l.id_operation_type, l.datetime 
        from "log" l, users u 
        where
            l.id_user = u.id
            and l.id_io_object = idObject
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;
