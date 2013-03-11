create or replace function recordAddToChain() returns trigger as
$BODY$
declare
    tableName varchar;
    idChain int4;
    idChainsData int4;
    idState int4;
    idObject int4;
    whatHappens int2;
    r record;
begin
    
    tableName := TG_RELNAME;
    if(substr(tableName, 1, 4) = 'tbl_') then
        tableName = substr(tableName, 5);
    end if;

                                                                                      
    if(TG_OP = 'INSERT') then
        idState = new.id_io_state;
        whatHappens := 4; --new EIO
    else
        if(old.id_io_state = new.id_io_state) then
            idState = 5; --system state Attr_changed
            whatHappens := 5; --update table attrs
        else 
            idState = new.id_io_state;
            whatHappens := 7; --change state of EIO
        end if;
    end if;
                               
    for r in 
        select c.id as id_c, io.id as id_o
        from chains c, tbl_io_objects io
        where 
            c.id_io_category = io.id_io_category 
            and c.id_io_state = idState 
            and io.table_name = tableName
    loop
        idChain := r.id_c;
        idObject := r.id_o;
    end loop;

    if(idChain isnull) then
        return new;
    end if;

    select getNextSeq('chains_data', 'id') into idChainsData;
    insert into chains_data (id, id_chain, id_io_object, id_record, insert_time, what_happens) 
    values (idChainsData, idChain, idObject, new.id, current_timestamp, whatHappens);

    return new;
end
$BODY$
language 'plpgsql' security definer;
