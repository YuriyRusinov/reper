--create payload and insert to table_notifies_log

create or replace function createNotify(varchar, varchar, int8, varchar, int4) returns varchar as
$BODY$
declare
    notifyName alias for $1;
    tableName alias for $2;
    idRecord alias for $3;
    recordUniqueId alias for $4;
    whatHappens alias for $5;

    idNotify int8;
    payload varchar;

    idUser int4;
    userName varchar;
    idPosition int4;
    positionName varchar;
    idOrg int4;
    orgName varchar;

    r record;
begin

    idNotify = getNextSeq('table_notifies_log', 'id');
    if(idNotify isnull or idNotify <= 0) then
        return NULL;
    end if;

    idUser = 0;
    for r in 
        select 
            u.id as id_user, 
            u.fio as user_name,
            p.id as id_pos,
            p.name as pos_name,
            uu.id as subsys_id,
            uu.name as subsys_name
        from 
            users u, 
            "position" p,
            units uu 
        where 
            u.role_name = session_user
            and u.id = p.id_user
            and p.id = getCurrentDl()
            and p.id_unit = uu.id
    loop
        idUser = r.id_user;
        userName = r.user_name;
        idPosition = r.id_pos;
        positionName = r.pos_name;
        idOrg = r.subsys_id;
        orgName = r.subsys_name;
    end loop;

    if(idUser = 0) then
        raise warning 'Cannot detect current user for current DL!';
        idUser = -1;
        userName = 'Undefined'; 
        idPosition = -1;
        positionName = 'Undefined';
        idOrg = -1;
        orgName = 'Undefined';
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
                                    invocation_datetime)
 
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
                                    current_timestamp);

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
              orgName;

    return payload;

end
$BODY$
language 'plpgsql' security definer;
