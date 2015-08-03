
create or replace function trgInsertAddress() returns trigger as
$BODY$
declare
    r record;
    full_address varchar;

    lastId int8;
begin

    if(new.house is null) then
        if(new.kl_code_kladr is null)  then
            return new;
        end if;

        lastId = getAddressLastId(new.kl_code_kladr);
        if(lastId isnull) then
            return new;
        end if;

        new.house = lastId;
    end if;

    for r in select * from getAddressStruct(new.house)
    loop
        new.region_str_fias = r.region;
        new.district_str_fial = r.auto;
        new.raion_str_fias = r.area;
        new.city_str_fias = r.city;
        new.city_raion_str_fias = r.ctar;
        new.town_str_fias = r.place;
        new.street_str_fias = r.street;
        new.house_part_str = r.house;
    end loop;

    select array_to_string(array_agg(offname), ',') into full_address from getAddressItemTree(new.house);
    new.addr_str_fias = full_address;
    
    for r in select regioncode, autocode, areacode, citycode, ctarcode, placecode, streetcode, extrcode, sextcode from tbl_eio_table_391 where id = lastId
    loop
        new.kl_code_kladr = r.regioncode || r.autocode || r.areacode || r.citycode || r.ctarcode || r.placecode || r.streetcode;
        if(r.extrcode is not null) then
            new.kl_code_kladr = new.kl_code_kladr || r.extrcode;
        end if;
        if(r.sextcode is not null) then
            new.kl_code_kladr = new.kl_code_kladr || r.sextcode;
        end if;
    end loop;

    return new;
    
end
$BODY$
language 'plpgsql' security definer;


--select f_safe_drop_trigger('trg_insert_address', 'tbl_eio_table_422');
--select f_create_trigger('trg_insert_address', 'before',  'insert or update', 'tbl_eio_table_422', 'trginsertaddress()');
