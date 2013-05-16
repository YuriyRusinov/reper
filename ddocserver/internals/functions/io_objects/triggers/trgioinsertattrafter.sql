create or replace function ioInsertAttrCheckAfter() returns trigger as
$BODY$
declare
    idChainsData int4;
    idChain int4;
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
            and p.id_state_dest = 5 --system state Attr_changed
            and io.id = new.id_io_object
--            and (io.is_completed = 1 or io.is_completed = 2)
    loop
        idChain = r.id;
        if(idChain isnull) then
            continue;
        end if;

        select getNextSeq('chains_data', 'id') into idChainsData;
        insert into chains_data (id, id_chain, id_io_object, insert_time, what_happens) 
        values (idChainsData, idChain, new.id_io_object, current_timestamp, 2); -- 2 - change of attributes of IO

    end loop;

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgioinsertattrafter', 'attrs_values');

select f_create_trigger('trgioinsertattrafter', 'after', 'insert or update', 'attrs_values', 'ioinsertattrcheckafter()');
