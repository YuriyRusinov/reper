
-- тип g_get_message_card объ€влен в getMessageCardAboutCamera.sql


/*
select * from getIncidentCard(2082);
select * from getIncidentMessages(1971)
select array_agg(mess_uuid::varchar) from getIncidentMessages(2082)
select * from getAdditionalInfo((select mess_uuid from getIncidentMessages(2082) limit 1))

select createTempTables();

select * from ioGetObjectIDByTableName('xarakter3')

select * from access_table where id_io_object = 395
select setCurrentDl(4);
select addAttributeToExistingQualifier(1037, 524, false, false, NULL);
*/

create or replace function getIncidentCard_1(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_490 where id = id_v;
    
    for r in select * from getIncidentCard_1(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getIncidentCard_1(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getIncidentCard_1(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getIncidentCard_1(uuid) returns setof h_get_message_card as
$BODY$
declare

    cardUuid alias for $1;
    
    r h_get_message_card%rowtype;
begin


    for r in 
    select 
        card.uuid_t::varchar as bus_uuid,
        it.uuid_t as type_uuid,
        it.incident_name as type_name,

        --координаты событи€
        card.latitude as latitude,
        card.longitude as longitude,
        NULL as altitude,

        --адрес событи€
        card.region_str as addr_reg_name,
        addr1.uuid_t as addr_reg_uuid,

        card.city_str as addr_city_name,
        addr2.uuid_t as addr_city_uuid,

        card.street_str as addr_street_name,
        addr3.uuid_t as addr_street_uuid,

        card.corps_str 
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        card.house_number_str as addr_house_number,
        addr5.uuid_t as addr_house_uuid,

        card.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        card.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        card.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        card.annotation as message_description,
        card.dop_inf as addr_additional_info,

        card.data_time_chs as msg_date,

        card.kontact as addressed_contact,
        card.fio as addressed_fio,

        'GELIOS' as external_sys_name
        
    from
        eio_table_490 card -- —правочник результатов расчета моделировани€
        left join eio_table_424 it on (card.incident = it.id) --  лассификатор событий устройств обнаружени€

        left join eio_table_391 addr1 on (card.region = addr1.id)
        left join eio_table_391 addr2 on (card.sity = addr2.id)
        left join eio_table_391 addr3 on (card.street = addr3.id)
        left join eio_table_391 addr5 on (card.house = addr5.id)

    where
        card.uuid_t = cardUuid

    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


/* ******************************/

select f_safe_drop_type('h_get_incident_messages');
create type h_get_incident_messages as(mess_uuid varchar, system_type varchar);


create or replace function getIncidentMessages(int8) returns setof h_get_incident_messages as 
$BODY$
declare
    id_v alias for $1;
    r h_get_incident_messages%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_490 where id = id_v;
    
    for r in select * from getIncidentMessages(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getIncidentMessages(varchar) returns setof h_get_incident_messages as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_incident_messages%rowtype;
begin

    for r in select * from getIncidentMessages(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getIncidentMessages(uuid) returns setof h_get_incident_messages as
$BODY$
declare
    cardUuid alias for $1;
    idCard int8;

    r h_get_incident_messages%rowtype;
begin

    select id into idCard from eio_table_490 where uuid_t = cardUuid;
    if(idCard isnull) then
        return;
    end if;


    for r in 
        select q.uuid_t, d.uuid_t::varchar --'MONITORING'
        from eio_table_490_eio_table_436_ref_1289 t, eio_table_436 q, eio_table_422 d
        where q.id = t.id_eio_table_436
              and t.id_eio_table_490 = idCard
              and q.device = d.id

        union all

        select q.uuid_t, d.uuid_t::varchar --'VIDEO'
        from eio_table_490_eio_table_439_ref_1288 t, eio_table_439 q, eio_table_422 d
        where q.id = t.id_eio_table_439
              and t.id_eio_table_490 = idCard
              and q.device = d.id
        
        union all

        select q.uuid_t, d.uuid_t::varchar --'FORECASTING'
        from eio_table_490_eio_table_438_ref_1290 t, eio_table_438 q, eio_table_436 e, eio_table_422 d
        where q.id = t.id_eio_table_438
              and t.id_eio_table_490 = idCard
              and q.event_id = e.id
              and e.device = d.id

        union all              

        select q.uuid_t, d.uuid_t::varchar --'MONITORING' (от аргуса)
        from eio_table_490_eio_table_492_ref_1293 t, eio_table_492 q, eio_table_422 d
        where q.id = t.id_eio_table_492
              and t.id_eio_table_490 = idCard
              and q.device = d.id

        union all              

        select q.uuid_t, q.uuid_t::varchar --'MONITORING' (от интернет-портала)
        from eio_table_490_fiks_incidents_1_ref_1329 t, fiks_incidents_1 q
        where q.id = t.id_fiks_incidents_1
              and t.id_eio_table_490 = idCard

    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

