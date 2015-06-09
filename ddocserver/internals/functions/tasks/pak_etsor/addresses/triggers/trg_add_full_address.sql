
create or replace function trgAddFullAddress() returns trigger as
$BODY$
declare
    r record;
    full_address varchar;

    lastId int8;
begin

    --if(new.addr_str_fias is not null and trim(new.addr_str_fias) <> '') then
    --    return new;
    --end if;
    
    lastId = new.id;
    for r in select * from getAddressStruct(lastId)
    loop
        full_address = r.full_address;
    end loop;

    if(new.house_number_str is not null and trim(new.house_number_str) <> '' and lower(new.house_number_str) <> 'нет' and trim(new.house_number_str) <> '-') then
        full_address = full_address || ', д. ' || new.house_number_str;
    end if;
    if(new.corps_str is not null and trim(new.corps_str) <> '' and lower(new.corps_str) <> 'нет' and trim(new.corps_str) <> '-') then
        full_address = full_address || ', корп. ' || new.corps_str;
    end if;
    if(new.building is not null and trim(new.building) <> '' and lower(new.building) <> 'нет' and trim(new.building) <> '-') then
        full_address = full_address || ', стр. ' || new.building;
    end if;


    new.addr_str_fias = full_address;

    return new;
       
end
$BODY$
language 'plpgsql' security definer;

create or replace function trgVerifyAndAddKlade() returns trigger as
$BODY$
declare
    r record;
    klade varchar;
    kladr varchar;
    fasetLength int4;

    lastId int8;
begin

    --if(new.oktmo is not null and trim(new.oktmo) <> '') then
    --    return new;
    --end if;

    if(new.code isnull and new.parent is not null) then
        select code into kladr from tbl_eio_table_391 where id = new.parent;
        new.code = kladr;
    end if;

    if(new.aolevel = 8 and new.parent is not null) then
        for r in  select regioncode, autocode, areacode, citycode, ctarcode, placecode, streetcode, extrcode, sextcode from tbl_eio_table_391 where id = new.parent
        loop
            new.regioncode = r.regioncode;
            new.autocode = r.autocode;
            new.areacode = r.areacode;
            new.citycode = r.citycode;
            new.ctarcode = r.ctarcode;
            new.placecode = r.placecode;
            new.streetcode = r.streetcode;
            new.extrcode = r.extrcode;
            new.sextcode = r.sextcode;

        end loop;
    end if;

    --regioncode
    if(new.regioncode isnull or trim(new.regioncode) = '') then
        new.regioncode = '00';
    end if;

    fasetLength = octet_length(new.regioncode);
    if(fasetLength = 1) then
        new.regioncode = '0' || new.regioncode;
    end if;
    if(fasetLength > 2) then
        new.regioncode = substring(new.regioncode from 1 for 2);
    end if;

    --autocode
    if(new.autocode isnull or trim(new.autocode) = '') then
        new.autocode = '0';
    end if;

    fasetLength = octet_length(new.autocode);
    if(fasetLength > 1) then
        new.autocode = substring(new.autocode from 1 for 1);
    end if;

    --areacode
    if(new.areacode isnull or trim(new.areacode) = '') then
        new.areacode = '000';
    end if;

    fasetLength = octet_length(new.areacode);
    if(fasetLength = 1) then
        new.areacode = '00' || new.areacode;
    end if;
    if(fasetLength = 2) then
        new.areacode = '0' || new.areacode;
    end if;
    if(fasetLength > 3) then
        new.areacode = substring(new.areacode from 1 for 3);
    end if;

    --citycode
    if(new.citycode isnull or trim(new.citycode) = '') then
        new.citycode = '000';
    end if;

    fasetLength = octet_length(new.citycode);
    if(fasetLength = 1) then
        new.citycode = '00' || new.citycode;
    end if;
    if(fasetLength = 2) then
        new.citycode = '0' || new.citycode;
    end if;
    if(fasetLength > 3) then
        new.citycode = substring(new.citycode from 1 for 3);
    end if;

    --ctarcode
    if(new.ctarcode isnull or trim(new.ctarcode) = '') then
        new.ctarcode = '000';
    end if;

    fasetLength = octet_length(new.ctarcode);
    if(fasetLength = 1) then
        new.ctarcode = '00' || new.ctarcode;
    end if;
    if(fasetLength = 2) then
        new.ctarcode = '0' || new.ctarcode;
    end if;
    if(fasetLength > 3) then
        new.ctarcode = substring(new.ctarcode from 1 for 3);
    end if;

    --placecode
    if(new.placecode isnull or trim(new.placecode) = '') then
        new.placecode = '000';
    end if;

    fasetLength = octet_length(new.placecode);
    if(fasetLength = 1) then
        new.placecode = '00' || new.placecode;
    end if;
    if(fasetLength = 2) then
        new.placecode = '0' || new.placecode;
    end if;
    if(fasetLength > 3) then
        new.placecode = substring(new.placecode from 1 for 3);
    end if;

    --streetcode
    if(new.streetcode isnull or trim(new.streetcode) = '') then
        new.streetcode = '0000';
    end if;

    fasetLength = octet_length(new.streetcode);
    if(fasetLength = 1) then
        new.streetcode = '000' || new.streetcode;
    end if;
    if(fasetLength = 2) then
        new.streetcode = '00' || new.streetcode;
    end if;
    if(fasetLength = 3) then
        new.streetcode = '0' || new.streetcode;
    end if;
    if(fasetLength > 4) then
        new.streetcode = substring(new.streetcode from 1 for 4);
    end if;

    --extrcode
    if(new.extrcode isnull or trim(new.extrcode) = '') then
        new.extrcode = '0000';
    end if;

    fasetLength = octet_length(new.extrcode);
    if(fasetLength = 1) then
        new.extrcode = '000' || new.extrcode;
    end if;
    if(fasetLength = 2) then
        new.extrcode = '00' || new.extrcode;
    end if;
    if(fasetLength = 3) then
        new.extrcode = '0' || new.extrcode;
    end if;
    if(fasetLength > 4) then
        new.extrcode = substring(new.extrcode from 1 for 4);
    end if;

    --sextcode
    if(new.sextcode isnull or trim(new.sextcode) = '') then
        new.sextcode = '000';
    end if;

    fasetLength = octet_length(new.sextcode);
    if(fasetLength = 1) then
        new.sextcode = '00' || new.sextcode;
    end if;
    if(fasetLength = 2) then
        new.sextcode = '0' || new.sextcode;
    end if;
    if(fasetLength > 3) then
        new.sextcode = substring(new.sextcode from 1 for 3);
    end if;


    klade = '';
    klade = klade || new.regioncode;
    klade = klade || new.autocode;
    klade = klade || new.areacode;
    klade = klade || new.citycode;
    klade = klade || new.ctarcode;
    klade = klade || new.placecode;
    klade = klade || new.streetcode;
    klade = klade || new.extrcode;
    klade = klade || new.sextcode;


    new.oktmo = klade;

    return new;
       
end
$BODY$
language 'plpgsql' security definer;

--update tbl_eio_table_391 set oktmo = NULL;
--select * from tbl_eio_table_391
--select f_safe_drop_trigger('xx_trg_add_full_address', 'tbl_eio_table_391'); --xx - чтобы последним триггер шел
--select f_create_trigger('xx_trg_add_full_address', 'before',  'insert or update', 'tbl_eio_table_391', 'trgaddfulladdress()');

--select f_safe_drop_trigger('xx_trg_verify_and_add_klade', 'tbl_eio_table_391'); --xx - чтобы последним триггер шел
--select f_create_trigger('xx_trg_verify_and_add_klade', 'before',  'insert or update', 'tbl_eio_table_391', 'trgverifyandaddklade()');
