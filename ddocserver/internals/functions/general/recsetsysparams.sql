create or replace function recSetSysParams(int8, int4, uuid, varchar, varchar, int8, int8) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idState alias for $2;
    uuidT alias for $3;
    uniqueId alias for $4;
    rIcon alias for $5;
    fillColor alias for $6;
    textColor alias for $7;

    tableName varchar;
    q varchar;
begin

    SELECT p.relname into tableName
    FROM q_base_table qq, pg_class p
    WHERE qq.id = idRec and qq.tableoid = p.oid;

    q = 'update "' || tableName || '" set id_io_state = ' || asString(idState, false) || 
                                            ', uuid_t = ' || asString(uuidT, true) ||
                                         ', unique_id = ' || asString(uniqueId, true) ||
                                            ', r_icon = ' || asString(rIcon, true) ||
                                 ', record_fill_color = ' || asString(fillColor, false) ||
                                 ', record_text_color = ' || asString(textColor, false) || 
                            ' where id = ' || idRec;

    execute q;
    if (not FOUND) then
        return -1;
    end if;

    return idRec;

end
$BODY$
language 'plpgsql';
