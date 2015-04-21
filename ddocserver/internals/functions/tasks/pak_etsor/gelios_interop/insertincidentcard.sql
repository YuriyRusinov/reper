
/*

select * from io_objects where id = 490
select * from io_categories where id = 487
select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 486);


select createTempTables();
select setCurrentDl(4);

assotiated_video;Ассоциированые события видеоаналитики
assotiated_hpost;Ассоциированные события гидропостов
assotiated_modeling;Ассоциированные события моделирования



Появилась функция создания (изменения) карточки происшествия.
Пока не реализована работа с additionalInfoList. Сейчас буду заниматься
Не неализовано также изменение карточки. Буду заниматься


create or replace function insertIncidentCard(varchar, --UUID _происшествия_(!!). При создании должен быть пуст (NULL). Если не NULL будем обновлять карточку
                                              varchar, -- тип происшествия. По классификатору 424-му. Пока используем старый классификатор, ибо на него все завязано
                                              float8, --координаты
                                              float8, 
                                              float8, 

                                              --адрес
                                              varchar, -- УИД региона
                                              varchar, --УИД города
                                              varchar, --УИД улицы
                                              varchar, --УИД доп. данные о номере дома (не используется пока, передаем NULL)
                                              varchar, --УИД номера дома 
                                              varchar,-- УИД парадной (не используется пока, передаем NULL)
                                              varchar,-- УИД этажа (не используется пока, передаем NULL)
                                              varchar,--УИД номера квартиры (не используется пока, передаем NULL)

                                              varchar,--описание происшествия
                                              varchar,--дополнительная информация об адресе

                                              timestamp with time zone, --дата-время происшествия
                                              varchar, -- контакты обратившегося
                                              varchar, --фио обратившегося
                                              varchar, --слово GELIOS (!!! и никакое другое)
                                              varchar[] --уникальные идентификаторы событий, ассоциированных с происшествием
                                              ) returns varchar as

Пример:
select insertIncidentCard( NULL, --(select uuid_t::varchar from eio_table_490 where id = 2114), 
                          (select uuid_t::varchar from eio_table_424 where id = 2017),
                          25.0::float8, 33.0::float8, 0.0::float8,
                          (select uuid_t::varchar from eio_table_391 where id = 542), -- region
                          (select uuid_t::varchar from eio_table_391 where id = 557), -- city
                          (select uuid_t::varchar from eio_table_391 where id = 581), -- street
                          NULL::varchar,
                          (select uuid_t::varchar from eio_table_391 where id = 597), -- house
                          NULL::varchar, NULL::varchar, NULL::varchar,

                          'новое происшествие'::varchar,
                          NULL::varchar,
                          now(),
                          'addressed_contact'::varchar, 'fio'::varchar, 'GELIOS'::varchar, 
                          (select array_agg(mess_uuid::varchar) from getIncidentMessages(2082))); 
*/

create or replace function insertIncidentCard(varchar, 
                                              varchar, 
                                              float8, 
                                              float8, 
                                              float8, 

                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,

                                              varchar,
                                              varchar,

                                              timestamp with time zone,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar[]
                                              ) returns varchar as
$BODY$
declare

    bus_uuid alias for $1;
    type_uuid alias for $2;
    lat alias for $3;
    lon alias for $4;
    altitude alias for $5;
    
    addr_reg_uuid  alias for $6;
    addr_city_uuid  alias for $7;
    addr_street_uuid  alias for $8;
    addr_house_number_add_uuid  alias for $9;
    addr_house_uuid  alias for $10;
    addr_house_part_uuid  alias for $11;
    addr_floor_uuid  alias for $12;
    addr_apartment_uuid  alias for $13;

    message_description  alias for $14;
    addr_additional_info  alias for $15;

    msg_date  alias for $16;
    addressed_contact  alias for $17;
    addressed_fio  alias for $18;

    external_sys_name  alias for $19;

    message_uuids alias for $20;

    card_uuid varchar;
    idCard int8;

    id_region int8;
    id_city int8;
    id_street int8;
    id_house int8;
    id_type int8;

    rCount int4;
    

begin

    if(external_sys_name <> 'GELIOS') then
        return NULL;
    end if;

    if(bus_uuid is not null) then
        return updateIncidentCard(bus_uuid,
                                  type_uuid,
                                  lat,
                                  lon,
                                  altitude,
                                  
                                  addr_reg_uuid,
                                  addr_city_uuid,
                                  addr_street_uuid,
                                  addr_house_number_add_uuid,
                                  addr_house_uuid,
                                  addr_house_part_uuid,
                                  addr_floor_uuid,
                                  addr_apartment_uuid,

                                  message_description,
                                  addr_additional_info,

                                  msg_date,
                                  addressed_contact,
                                  addressed_fio,
                                  
                                  external_sys_name,
                                  message_uuids);
    end if;


    select id into id_region from eio_table_391 where uuid_t = addr_reg_uuid::uuid;
    select id into id_city from eio_table_391 where uuid_t = addr_city_uuid::uuid;
    select id into id_street  from eio_table_391 where uuid_t = addr_street_uuid::uuid;
    select id into id_house from eio_table_391 where uuid_t = addr_house_uuid::uuid;
    select id into id_type from eio_table_424 where uuid_t = type_uuid::uuid;

    select getNextSeq('eio_table_490', 'id') into idCard;

    insert into eio_table_490 (id, 
                               latitude,
                               longitude,
                               
                               region,
                               autonom,
                               sity,
                               sity_rajon,
                               punkt,
                               street,
                               obl,
                               house,
                               dop_inf,

                               annotation,
                               
                               data_time_kart,  
                               data_time_in_data,
                               data_time_chs,
                     
                               kontact, --Контакты обратившегося
                               fio, --ФИО обратившегося и (или) название организ.
                               
                               type_chs3, --Тип ЧС
                               incident,
                               guuid_chs3) --НАИМЕНОВАНИЕ ПРОИСШЕСТВИЯ(ЧС)
                         values(idCard, 
                                lat, lon, 
                                id_region, NULL, id_city, NULL, NULL, id_street, NULL, id_house, addr_additional_info, 
                                message_description, 
                                msg_date, msg_date, msg_date, 

                                addressed_contact, addressed_fio, 
                                
                                96, --Тип ЧС. Новый тип. Пока не используем. Проставляем значение по умолчанию
                                id_type, --Тип ЧС (старый)
                                type_uuid);

    --get diagnostics rCount = ROW_COUNT;

    --if(rCount <= 0) then
    --    return NULL;
    --end if;

    select uuid_t::varchar into card_uuid from eio_table_490 where id = idCard;

    if(card_uuid is not null) then
        perform insertIncidentMessages(idCard, message_uuids);
    end if;

    return card_uuid;
end
$BODY$
language 'plpgsql';

create or replace function updateIncidentCard(varchar, 
                                              varchar, 
                                              float8, 
                                              float8, 
                                              float8, 

                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar,

                                              varchar,
                                              varchar,

                                              timestamp with time zone,
                                              varchar,
                                              varchar,
                                              varchar,
                                              varchar[]
                                              ) returns varchar as
$BODY$
declare

    bus_uuid alias for $1;
    type_uuid alias for $2;
    lat alias for $3;
    lon alias for $4;
    altitude alias for $5;
    
    addr_reg_uuid  alias for $6;
    addr_city_uuid  alias for $7;
    addr_street_uuid  alias for $8;
    addr_house_number_add_uuid  alias for $9;
    addr_house_uuid  alias for $10;
    addr_house_part_uuid  alias for $11;
    addr_floor_uuid  alias for $12;
    addr_apartment_uuid  alias for $13;

    message_description  alias for $14;
    addr_additional_info  alias for $15;

    msg_date  alias for $16;
    addressed_contact  alias for $17;
    addressed_fio  alias for $18;

    external_sys_name  alias for $19;
    message_uuids alias for $20;

    card_uuid varchar;
    idCard int8;

    id_region int8;
    id_city int8;
    id_street int8;
    id_house int8;
    id_type int8;

    rCount int4;
    

begin

    if(external_sys_name <> 'GELIOS') then
        return NULL;
    end if;

    if(bus_uuid is null) then
        return insertIncidentCard(bus_uuid,
                                  type_uuid,
                                  lat,
                                  lon,
                                  altitude,
                                  
                                  addr_reg_uuid,
                                  addr_city_uuid,
                                  addr_street_uuid,
                                  addr_house_number_add_uuid,
                                  addr_house_uuid,
                                  addr_house_part_uuid,
                                  addr_floor_uuid,
                                  addr_apartment_uuid,

                                  message_description,
                                  addr_additional_info,

                                  msg_date,
                                  addressed_contact,
                                  addressed_fio,
                                  
                                  external_sys_name,
                                  message_uuids);
    end if;


    select id into id_region from eio_table_391 where uuid_t = addr_reg_uuid::uuid;
    select id into id_city from eio_table_391 where uuid_t = addr_city_uuid::uuid;
    select id into id_street  from eio_table_391 where uuid_t = addr_street_uuid::uuid;
    select id into id_house from eio_table_391 where uuid_t = addr_house_uuid::uuid;
    select id into id_type from eio_table_424 where uuid_t = type_uuid::uuid;


    update eio_table_490 set
    
        latitude = lat,
        longitude = lon,

        region = id_region,
        autonom = NULL,
        sity = id_city,
        sity_rajon = NULL,
        punkt = NULL,
        street = id_street,
        obl = NULL,
        house = id_house,
        dop_inf = addr_additional_info,

        annotation = message_description,

        data_time_kart = msg_date,  
        data_time_in_data = msg_date,
        data_time_chs = msg_date,
        
        kontact = addressed_contact, --Контакты обратившегося
        fio = addressed_fio, --ФИО обратившегося и (или) название организ.
                               
        type_chs3 = 96, --Тип ЧС
        incident = id_type,
        guuid_chs3 = type_uuid --НАИМЕНОВАНИЕ ПРОИСШЕСТВИЯ(ЧС)
     where uuid_t = bus_uuid::uuid;


    select id into idCard from eio_table_490 where uuid_t = bus_uuid::uuid;
    if(idCard is not null) then
        perform insertIncidentMessages(idCard, message_uuids);
    end if;


    return bus_uuid;
end
$BODY$
language 'plpgsql';



create or replace function insertIncidentMessages(int8, varchar[]) returns int4 as
$BODY$
declare
    idCard alias for $1;
    uuids alias for $2;
    
    tableName varchar;
    idMessage int8;
    r record;
begin


    delete from eio_table_490_eio_table_436_ref_1289 where id_eio_table_490 = idCard;
    delete from eio_table_490_eio_table_438_ref_1290 where id_eio_table_490 = idCard;
    delete from eio_table_490_eio_table_439_ref_1288 where id_eio_table_490 = idCard;
    delete from eio_table_490_eio_table_492_ref_1293 where id_eio_table_490 = idCard;
    delete from eio_table_490_fiks_incidents_1_ref_1329 where id_eio_table_490 = idCard;

    for r in select unnest(uuids) as uid
    loop
        tableName = getRecordTableByUuid(r.uid);
        if(tableName = 'eio_table_436') then
            select id into idMessage from eio_table_436 where uuid_t = r.uid::uuid;
            if(idMessage isnull) then
                raise warning 'Undefined message on incident was found (id was not found by uuid)! Skipped!';
                continue;
            end if;
            
            insert into eio_table_490_eio_table_436_ref_1289 (id_eio_table_436, id_eio_table_490) values (idMessage, idCard);
            
        elsif (tableName = 'eio_table_438') then

            select id into idMessage from eio_table_438 where uuid_t = r.uid::uuid;
            if(idMessage isnull) then
                raise warning 'Undefined message on incident was found (id was not found by uuid)! Skipped!';
                continue;
            end if;

            insert into eio_table_490_eio_table_438_ref_1290 (id_eio_table_438, id_eio_table_490) values (idMessage, idCard);

        elsif(tableName = 'eio_table_439') then

            select id into idMessage from eio_table_439 where uuid_t = r.uid::uuid;
            if(idMessage isnull) then
                raise warning 'Undefined message on incident was found (id was not found by uuid)! Skipped!';
                continue;
            end if;

            insert into eio_table_490_eio_table_439_ref_1288 (id_eio_table_439, id_eio_table_490) values (idMessage, idCard);

        elsif(tableName = 'eio_table_492') then

            select id into idMessage from eio_table_492 where uuid_t = r.uid::uuid;
            if(idMessage isnull) then
                raise warning 'Undefined message on incident was found (id was not found by uuid)! Skipped!';
                continue;
            end if;

            insert into eio_table_490_eio_table_492_ref_1293 (id_eio_table_492, id_eio_table_490) values (idMessage, idCard);

        elsif(tableName = 'fiks_incidents_1') then

            select id into idMessage from fiks_incidents_1 where uuid_t = r.uid::uuid;
            if(idMessage isnull) then
                raise warning 'Undefined message on incident was found (id was not found by uuid)! Skipped!';
                continue;
            end if;

            insert into eio_table_490_fiks_incidents_1_ref_1329 (id_fiks_incidents_1, id_eio_table_490) values (idMessage, idCard);

        else
            raise warning 'Undefined message on incident was found! Skipped!';
            continue;
        end if;

        
        
    end loop;

    

    return 1;
end
$BODY$
language 'plpgsql';


--select unnest(ARRAY[1, 2, 3]);
