create or replace function ioInsertAttrCheckAfter() returns trigger as
$BODY$
declare
    idChainsData int4;
    idChain int4;
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

    select c.id into idChain 
    from chains c, io_processing_order p, tbl_io_objects io
    where 
        p.id_io_category = io.id_io_category 
        and p.id_chain = c.id
        and p.id_state_dest = 5 --system state Attr_changed
        and io.id = new.id_io_object;

    if(idChain isnull) then
        return new;
    end if;

    select getNextSeq('chains_data', 'id') into idChainsData;
    insert into chains_data (id, id_chain, id_io_object, insert_time, what_happens) 
    values (idChainsData, idChain, new.id_io_object, current_timestamp, 2); -- 2 - change of attributes of IO

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgioinsertattrafter', 'attrs_values');

select f_create_trigger('trgioinsertattrafter', 'after', 'insert or update', 'attrs_values', 'ioinsertattrcheckafter()');
