create or replace function ioUpdateTable (varchar, int4, int4, varchar, varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    idObject alias for $2;
    idCategory alias for $3;
    tName alias for $4;
    uniqueid alias for $5;

    res int4;
    idTable int4;
    tNameJ varchar;
begin

    select id into idTable from object_ref_tables where unique_id = uniqueId;
    if(idTable is null) then
        idTable = ioInsertTable(tableName, idObject, idCategory, tName, uniqueId);
        if(idTable <= 0) then
            return -1;
        end if;
        return 1;
    end if;

    update object_ref_tables set title = tName where id = idTable;

    if(FOUND = FALSE) then
        return -1;
    end if;

    return 1;

end
$BODY$ 
language 'plpgsql';
