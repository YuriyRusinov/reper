create or replace function getIDByUID(varchar, varchar) returns int8 as
$BODY$
declare
    tableName alias for $1;
    uid alias for $2;

    q varchar;
    r record;
    uidColumn varchar;

    idObject int4;
    idRecord int8;
    tName varchar;
begin
    if(tableName isnull) then
        return NULL;
    end if;

    if(tableName = 'organization') then
        uidColumn = 'email_prefix';
    else 
        uidColumn = 'unique_id';
    end if;
    
    q = 'select id::int8 from ' || tableName || ' where ' || uidColumn || ' = ' || quote_literal(uid);
    for r in execute q
    loop
        return r.id;
    end loop;

    if(FOUND = FALSE) then
        --нет записей с заданным UID
        --проверим, не является ли данный справочник системным.
        --если является, то скорее всего в нем есть нужная запись просто с другим UID (локального "происхождения")
        select id into idObject from tbl_io_objects where table_name = tableName; 
        if(idObject isnull) then
            return NULL;
        end if;

        if(idObject > 300) then
            return NULL;
        end if;
        
        idRecord = to_int8(split_part(uid, '-', 3)); --email_prefix-table_name-ID --берем третью часть, которая представляет собой ID
        tName = split_part(uid, '-', 2); --берем вторую часть (table_name)

        if(tableName <> tName) then
            return NULL;
        end if;

        q = 'select id::int8 from ' || tableName || ' where id = ' || idRecord;
        for r in execute q
        loop
            idRecord = r.id;
        end loop;
        
        if (FOUND = FALSE) then
            return NULL;
        end if;
        
        return idRecord;
    end if;

    return NULL;

end
$BODY$
language 'plpgsql' security definer;
