---важно!
--необходимо учесть обработку атрибутов типа atCheckListEx!
--сейчас здесь это НЕ делается

create or replace function updaterecord (varchar, int8, int4[], varchar[]) returns int8 as
$BODY$
declare
    tableName alias for $1;
    idRec alias for $2;
    attrs alias for $3;
    values alias for $4;

    query varchar;
    nattrs int4;
    nattr_vals int4;
    r RECORD;
    rr RECORD;
    ct int4;
    i int4;
    nid int4;
    idval int4;
    attr_query varchar;
    acode varchar;
    attr_type varchar;
    attr_type_query varchar;
    is_updated boolean;
begin
    if (tableName is null or attrs is null or values is null) then
        raise notice 'invalid parameters';
        return -1;
    end if;

    nattrs := array_upper (attrs, 1);
    nattr_vals := array_upper (values, 1);

    raise notice 'number of attributes is %, number of values is %', nattrs, nattr_vals;
    if (nattrs <> nattr_vals) then
        raise notice 'number of attributes has to be equal number of values';
        return -1;
    end if;
    ct := 0;
    query := 'select count(*) from pg_tables where tablename =' || quote_literal (tableName);
    for r in
        execute query
    loop
        ct := r.count;
    end loop;
    raise notice 'tables number %', ct;
    if (ct = 0) then
        raise notice 'table % does not find in database', tableName;
        return -1;
    end if;
    query := 'UPDATE ' || tableName || ' SET ';
    for i in 1..nattrs
    loop
        attr_query := 'select a.code from attributes a where id=' || attrs [i];
        for r in
            execute attr_query
        loop
            acode := r.code;
            raise notice 'attribute code is %', acode;
            if (acode <> 'id') then
                query := query || acode || ' = '; --|| values[i];
                attr_type_query := 'select at.code from a_types at inner join attributes a on (a.id_a_type=at.id and a.id=' || attrs [i] || ' )';
                for rr in 
                    execute attr_type_query
                loop
                    attr_type := rr.code;
                end loop;
                if (lower (attr_type) = 'varchar' or lower (attr_type) = 'text') then
                    query := query || quote_literal (values[i]);
                elsif (lower (attr_type) = 'int4' or lower (attr_type) = 'float8') then
                    query := query || values[i];
                elsif (lower (attr_type) = lower ('DATE') or lower (attr_type) = 'timestamp') then
                    query := query || values[i] || '::timestamp';
                elsif (lower (attr_type) = lower ('INTERVAL')) then
                    query := query || values[i] || '::interval';
                end if;

                if (i < nattrs) then
                    query := query || ', ';
                end if;
            else
                nid := i;
            end if;
        end loop;
    end loop;
    query := query || ' where id=' || idRec;

--    raise notice '%', query;
    execute query;

--    is_updated := FOUND;
--    raise notice '%', is_updated;
--    if (is_updated) then
        return idRec;
--    else
--        return -1;
--    end if;
end
$BODY$
language 'plpgsql';
