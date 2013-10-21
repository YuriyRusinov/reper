create or replace function addQueueResult (int8, int4, varchar, int4, varchar, bool) returns int8 as
$BODY$
declare
    idQueue alias for $1;
    syncResult alias for $2;
    sentAddr alias for $3;
    sentPort alias for $4;
    orgUid alias for $5;
    useGateway alias for $6;

    idRes int8;
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

    query := 'select id from queue_results where id_external_queue = ' || idQueue || ' and address = ' || quote_literal (sentAddr);
    if(sentPort is not null) then
        query := query || ' and port = ' || sentPort;
    end if;

    for r in
        execute query
    loop
        idRes = r.id;
    end loop;

    if (idRes is not null) then
        uquery := 'update queue_results set sync_result = ' || syncResult || ' where id = ' || r.id;
        execute uquery;
        return idRes;
    end if;

    select into idRes getNextSeq('queue_results', 'id');

    query := 'insert into queue_results (id, id_transport, id_external_queue, sync_result, address, port, org_uid, use_gateway) values (';
    query := query || idRes || ',' || idTransport || ',' || idQueue || ',' || syncResult || ',' || quote_literal (sentAddr) || ',' || sentPort || ',' || quote_literal(orgUid) || ', ' || asString(useGateway, false) || ');';

--    raise notice 'query is %', query;
    execute query;
    if (not FOUND) then
        return -1;
    end if;

    return idRes;

end
$BODY$
language 'plpgsql';
