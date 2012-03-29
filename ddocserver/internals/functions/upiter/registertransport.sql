create or replace function __registerTr() returns int4 as
$BODY$
declare
begin

    create temp table curr_transport (id_transport int4);

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function __unregisterTr() returns int4 as
$BODY$
declare
begin

    drop table curr_transport;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function registerTransport(int4) returns int4 as
$BODY$
declare
    idTransport alias for $1;
begin

    perform __registerTr();
    insert into curr_transport values(idTransport);

    update transport set is_active = TRUE where id = idTransport;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function unregisterTransport() returns int4 as
$BODY$
declare

    cnt int4;
    idTransport int4;
begin

    select id_transport into idTransport from curr_transport limit 1;

    update transport set is_active = FALSE where id = idTransport;

    perform __unregisterTr();

    return 1;
end
$BODY$
language 'plpgsql';
