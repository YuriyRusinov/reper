CREATE OR REPLACE FUNCTION notifyQualifier() RETURNS trigger AS
$BODY$
declare

    tableName varchar;
    recordUniqueId varchar;
    recordUuid uuid;
    idRecord int8;
    whatHappens int4;

    payload varchar;

    --params and criteria for notify generation
    notifyName varchar; 
    idPositionParam int4; 
    idUnitParam int4;
    idSearchTemplate int4;
    isAccept bool;

    positionName varchar;
    idUser int4;
    idPosition int4;
    idUnit int4;
    userName varchar;
    unitName varchar;

    idCurrentDl int4;
    query varchar;
    filterQuery varchar;
    filterAccepted bool;
    r record;
begin

    if(TG_NARGS < 5) then
        raise exception 'incorrect trigger functuion calling';
        return NULL;
    end if;

    raise warning 'start to analyzing params for sending notify';

    notifyName = TG_ARGV[0];    
    if(lower(TG_ARGV[1]) = 'null') then
        idPositionParam = NULL;
    else
        idPositionParam = TG_ARGV[1];
    end if;

    if(lower(TG_ARGV[2]) = 'null') then
        idUnitParam = NULL;
    else
        idUnitParam = TG_ARGV[2];
    end if;

    if(lower(TG_ARGV[3]) = 'null') then
        idSearchTemplate = NULL;
    else
        idSearchTemplate = TG_ARGV[3];
    end if;

    isAccept = TG_ARGV[4];

    tableName := TG_RELNAME;
    if(substr(tableName, 1, 4) = 'tbl_') then
        tableName = substr(tableName, 5);
    end if;

                                                                                      
    if(TG_OP = 'INSERT') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        recordUuid = new.uuid_t;
        whatHappens := 1; --new EIO
    elsif(TG_OP = 'UPDATE') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        recordUuid = new.uuid_t;
        whatHappens := 2; --updated EIO
    else
        idRecord = old.id;
        recordUniqueId = old.unique_id;
        recordUuid = old.uuid_t;
        whatHappens := 3; --updated EIO
    end if;

    idCurrentDl = getCurrentDl();
    if(idPositionParam is not null and idPositionParam <> idCurrentDl) then

        raise warning 'Notify should be generated only for idPosition = %. Current idPosition = %', idPositionParam, idCurrentDl;

        if(TG_OP = 'DELETE') then
            return old;
        end if;

        return new;
    end if;

    -- in DELETE operations id_search_template is not used!
    if(TG_OP <> 'DELETE') then
        if(idSearchTemplate is not null) then
        
            filterQuery = generateFilterQuery(asString(tableName, false), idSearchTemplate);
            raise warning 'Notify should be generated only if given search template is accepted. Query = %', filterQuery;

            if(filterQuery is null) then
                raise warning 'filter query for the search template (idSearchTemplate = %) is NULL! Notify should not be generated!', idSearchTemplate;

                if(TG_OP = 'DELETE') then
                    return old;
                end if;

                return new;
            end if;

            query = 'select id from "' || tableName || '" where id =  ' || new.id || ' ' || filterQuery;

            filterAccepted = false;
            for r in execute query
            loop
                if(r.id = new.id) then
                    filterAccepted = true;
                end if;
            end loop;

            if(filterAccepted = false) then
                raise warning 'Current record of qualifier does not correspond to filter query for the search template (idSearchTemplate = %)! Notify should not be generated!', idSearchTemplate;

                if(TG_OP = 'DELETE') then
                    return old;
                end if;

                return new;
            end if;

        end if;
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
            and p.id = idCurrentDl
            and p.id_unit = uu.id
    loop
        idUser = r.id_user;
        userName = r.user_name;
        idPosition = r.id_pos;
        positionName = r.pos_name;
        idUnit = r.subsys_id;
        unitName = r.subsys_name;
    end loop;

    if(idUser = 0) then
        raise warning 'Cannot detect current user for current idPosition! current idPosition = %', idCurrentDl;

        idUser = -1;
        userName = 'Undefined'; 
        idPosition = -1;
        positionName = 'Undefined';
        idUnit = -1;
        unitName = 'Undefined';
    end if;

    if(idUnitParam is not null and idUnitParam <> idUnit) then

        raise warning 'Notify should be generated only for idUnit = %. Current idUnit = %', idUnitParam, idUnit;

        if(TG_OP = 'DELETE') then
            return old;
        end if;

        return new;
    end if;


    if(isAccept = false) then

        raise warning 'All params for notify generation is accepted, but param IS_ACCETPT = FALSE. Should not generate notify!';

        if(TG_OP = 'DELETE') then
            return old;
        end if;

        return new;
    end if;

    payload = createNotify(notifyName, tableName, idRecord, recordUniqueId, whatHappens, idUser, userName, idPosition, positionName, idUnit, unitName, recordUuid::varchar);
    if(payload isnull) then
        raise warning 'payload for notify is NULL! Nothing to generate!';
        return NULL;
    end if;
                               
    perform pg_notify(notifyName, payload);

    return new;
end
$BODY$ 
language 'plpgsql' SECURITY DEFINER;
