
create or replace function ioInsertAttrCheckAfter() returns trigger as
$BODY$
declare
    idChainsData int4;
    idChain int4;
    r record;

    idType int4;
    res int4;

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

    --complex attrs

    if(TG_OP = 'INSERT') then
        --for complex attributes we need to setup values for all childs (in rec_attrs_attrs_values)
        -- by parsing new.value
        -- format is: id~~~val^~^~^id~~~val^~^~^id~~~val ...
        select 
            a.id_a_type into idType 
        from 
            attributes a,
            attrs_categories ac
        where
            a.id = ac.id_io_attribute
            and ac.id = new.id_attr_category;

        if(idType isnull) then
            raise exception 'Found attribute with NULL type!';
            return NULL;
        end if;

        if(idType = 32) then --complex attribute
            /* --invoked in before trigger
            if(session_user = 'jupiter') then --information exchange. in that case new.value contains unique_id of attr_attr. We should convert it to ID of attr_attr
                new.value = convertAttrAttrValue(new.value);
            end if;
            */

            res = insertAttrsAttrsValues(new.id, new.value);
            if(res <= 0) then
                raise exception 'Cannot parse and setup data in attrs_attrs_values!';
                return NULL;
            end if;
        end if;
    end if;



    --for chains

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
