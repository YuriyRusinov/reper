--create payload and insert to table_notifies_log

create or replace function createNotify(varchar, varchar, int8, varchar, int4, int4, varchar, int4, varchar, int4, varchar, varchar) returns varchar as
$BODY$
declare
    notifyName alias for $1;
    tableName alias for $2;
    idRecord alias for $3;
    recordUniqueId alias for $4;
    whatHappens alias for $5;

    idUser alias for $6;
    userName alias for $7;
    idPosition alias for $8;
    positionName alias for $9;
    idOrg alias for $10;
    orgName alias for $11;
    uuidT alias for $12;

    idNotify int8;
    payload varchar;

    r record;
begin

    idNotify = getNextSeq('table_notifies_log', 'id');
    if(idNotify isnull or idNotify <= 0) then
        return NULL;
    end if;

    insert into table_notifies_log (id,
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
                                    invocation_datetime,
                                    record_uuid)
 
                             values(idNotify,
                                    notifyName,
                                    idUser,
                                    userName,
                                    idPosition,
                                    positionName,
                                    idOrg,
                                    orgName,
                                    tableName,
                                    idRecord,
                                    whatHappens,
                                    current_timestamp,
                                    uuidT);

    payload = tableName || '~_~_~' || 
              idRecord || '~_~_~' || 
              recordUniqueId || '~_~_~' || 
              whatHappens || '~_~_~' || 
              idNotify || '~_~_~' || 
              idUser || '~_~_~' || 
              userName || '~_~_~' || 
              idPosition || '~_~_~' || 
              positionName || '~_~_~' || 
              idOrg || '~_~_~' || 
              orgName || '~_~_~' || 
              uuidT || '~_~_~' ||
              notifyName;

    return payload;

end
$BODY$
language 'plpgsql' security definer;
