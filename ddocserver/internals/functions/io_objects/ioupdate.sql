create or replace function ioUpdate(int4, varchar, int4, varchar, varchar, int4, int4, boolean, int8, int8, int4, varchar) returns int4 as
$BODY$
declare

    idObject alias for $1;
    ioName alias for $2;
    idState alias for $3;
    ioDesc alias for $4;
    ioInfo alias for $5;
    idMaclabel alias for $6;
    idSyncType alias for $7;
    isGlobal alias for $8;
    recordFillColor alias for $9;
    recordTextColor alias for $10;
    idType alias for $11;
    rIcon alias for $12;


begin

    if (idObject is null or idObject <= 0) then
        return -1;
    end if;
    
    update io_objects set 
                            --id_io_category = , IO cannot change category
                            --author, IO cannot change author
                            id_io_state = idState, 
                            id_io_type = idType,
                            name = ioName, 
                            --table_name = tableName, 
                            description = ioDesc, 
                            information = ioInfo, 
                            --is_system, 
                            --insert_time,
                            id_maclabel = idMaclabel,
                            --unique_id,
                            id_sync_type = idSyncType,
                            --id_owner_org,
                            is_global = isGlobal,
                            record_fill_color = recordFillColor,
                            record_text_color = recordTextColor,
                            r_icon = rIcon,
                            last_update = CURRENT_TIMESTAMP
    where id=idObject;


    return idObject;

end
$BODY$
language 'plpgsql';
