create or replace function addQueueResult (int4, int4, varchar) returns int4 as
$BODY$
declare
    idQueue alias for $1;
    syncResult alias for $2;
    sentAddr alias for $3;

    id int4;
    idTransport int4;
    query varchar;
    uquery varchar;
    cnt int4;
    r record;
begin

    select into idTransport getCurrentTransport ();
    if (idTransport is null or idTransport < 0) then
        raise notice 'Current transport is not set';
        return -1;
    end if;
    query := 'select id from queue_results where id_external_queue = ' || idQueue || 'and address = ' || quote_literal (sentAddr);

    for r in
        execute query
    loop
        id = r.id;
    end loop;
    if (id is not null) then
        uquery := 'update queue_results set sync_result = ' || syncResult || 'where id = ' || r.id;
        execute uquery;
        return id;
    end if;

    select into id getNextSeq('queue_results', 'id');
    query := 'insert into queue_results (id, id_transport, id_external_queue, sync_result, address) values (';
    query := query || id || ',' || idTransport || ',' || idQueue || ',' || syncResult || ',' || quote_literal (sentAddr) || ');';

--    raise notice 'query is %', query;
    execute query;
    if (not FOUND) then
        return -1;
    else
        return id;
    end if;
end
$BODY$
language 'plpgsql';
