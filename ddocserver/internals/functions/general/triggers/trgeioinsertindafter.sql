create or replace function eioInsertIndCheckAfter() returns trigger as
$BODY$
declare
    idChainsData int4;
    idChain int4;
    tableName varchar;
    r record;
begin

    --chains
    if(new.is_actual = false) then  -- skip archived data
        return new;
    end if;

    if(TG_OP = 'UPDATE') then
        if(old.value = new.value) then
            return new;
        end if;
    end if;

    SELECT p.relname into tableName
    FROM q_base_table q, pg_class p
    WHERE q.id = new.id_record AND q.tableoid = p.oid;
    
    for r in 
        select 
            c.id 
        from 
            chains c, 
            io_processing_order p, 
            io_processing_order_chains cio,
            tbl_io_objects io
        where 
            p.id_io_category = io.id_io_category 
            and cio.id_chains = c.id
            and cio.id_io_processing_order = p.id
            and p.id_state_dest = 5 --system state attr_changed
            and io.table_name = tableName
    loop
        idChain = r.id;

        if(idChain isnull) then
            continue;
        end if;

        select getNextSeq('chains_data', 'id') into idChainsData;

        insert into chains_data (id, id_chain, id_record, insert_time, what_happens) 
        values (idChainsData, idChain, new.id_record, current_timestamp, 6); -- 6 - change of indicators of EIO

    end loop;

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgeioinsertindafter', 'rec_attrs_values');

select f_create_trigger('trgeioinsertindafter', 'after', 'insert or update', 'rec_attrs_values', 'eioinsertindcheckafter()');
