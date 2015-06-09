
create or replace function trgAddFullAddressHouse() returns trigger as
$BODY$
declare
    r record;
    full_address varchar;

    lastId int8;
begin

    if(new.adress is not null and trim(new.adress) <> '') then
        return new;
    end if;
    
    if(new.street isnull) then
        return new;
    end if;

    select addr_str_fias into full_address from tbl_eio_table_391 where id = new.street;
    if(full_address isnull or trim(full_address) = '') then
        return new;
    end if;

    if(new.house is not null and trim(new.house) <> '' and lower(new.house) <> 'нет' and trim(new.house) <> '-') then
        full_address = full_address || ', д. ' || new.house;
    end if;
    if(new.korpus is not null and trim(new.korpus) <> '' and lower(new.korpus) <> 'нет' and trim(new.korpus) <> '-') then
        full_address = full_address || ', корп. ' || new.korpus;
    end if;
    if(new.stroenie is not null and trim(new.stroenie) <> '' and lower(new.stroenie) <> 'нет' and trim(new.stroenie) <> '-') then
        full_address = full_address || ', стр. ' || new.stroenie;
    end if;

    new.adress = full_address;

    return new;
       
end
$BODY$
language 'plpgsql' security definer;


--select * from tbl_house_617
--update tbl_house_617 set adress = '';
--select f_safe_drop_trigger('xx_trg_add_full_address_house', 'tbl_house_617'); --xx - чтобы последним триггер шел
--select f_create_trigger('xx_trg_add_full_address_house', 'before',  'insert or update', 'tbl_house_617', 'trgaddfulladdresshouse()');
