select f_safe_drop_type('h_get_table_notify');
create type h_get_table_notify as(
                                  id_notify int8,
                                  notify_name varchar,
                                  id_user int4,
                                  user_name varchar,
                                  id_position int4, 
                                  position_name varchar,
                                  id_org int4,
                                  org_name varchar,
                                  table_name varchar,
                                  id_record int8,
                                  what_happens int4,
                                  invocation_datetime timestamp);

create or replace function getTableNotify(int8) returns setof h_get_table_notify as
$BODY$
declare
    idNotify alias for $1;
    r h_get_table_notify%rowtype;
begin

    for r in select id,
                    notify_name, 
                    id_user, 
                    user_name, 
                    id_position, 
                    position_name, 
                    id_org, 
                    org_name, 
                    table_name, 
                    id_record, 
                    what_happens,
                    invocation_datetime 
              from table_notifies_log
              where id = idNotify
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

