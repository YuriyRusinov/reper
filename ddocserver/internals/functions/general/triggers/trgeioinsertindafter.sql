create or replace function eioInsertIndCheckAfter() returns trigger as
$BODY$
declare
    idChainsData int4;
    idChain int4;
    tableName varchar;
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

    select c.id into idChain 
    from chains c, tbl_io_objects io
    where 
        c.id_io_category = io.id_io_category 
        and c.id_io_state = 5 --system state attr_changed
        and io.table_name = tableName;

    if(idChain isnull) then
        return new;
    end if;

    select getNextSeq('chains_data', 'id') into idChainsData;
    insert into chains_data (id, id_chain, id_record, insert_time, what_happens) 
    values (idChainsData, idChain, new.id_record, current_timestamp, 6); -- 6 - change of indicators of EIO

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgeioinsertindafter', 'rec_attrs_values');

select f_create_trigger('trgeioinsertindafter', 'after', 'insert or update', 'rec_attrs_values', 'eioinsertindcheckafter()');
