/*
select * from eio_table_436;
select * from eio_table_438;

select * from eio_table_439;
select * from eio_table_424;
select * from eio_table_422;
select * from eio_table_391;
select * from units;

select * from fiks_incidents_1

--select * from attributes where id = 1198
select * from getMessageCardAboutCamera(918);
select * from getMessageCardAboutHPost(1998);
select * from getMessageCardAboutModeling(694);
select * from getMessageCardAboutPortal(4148);
update units set code_name = 'MONITORING' where id = 19;
select * from eio_table_492
--select * from "position" where id = 23
select * from units;
select * from getMessageCardAboutFire(2195);

*/

--create

select f_safe_drop_type('h_get_message_card');
create type h_get_message_card as(bus_uuid varchar,
                                  type_uuid varchar,
                                  type_name varchar,

                                  latitude float8,
                                  longitude float8,
                                  altitude float8,
                                  
                                  addr_reg_name varchar,
                                  addr_reg_uuid varchar,
                                  addr_city_name varchar,
                                  addr_city_uuid varchar,
                                  addr_street_name varchar,
                                  addr_street_uuid varchar,
                                  addr_house_number_add varchar,
                                  addr_house_number_add_uuid varchar,
                                  addr_house_number varchar,
                                  addr_house_uuid varchar,
                                  addr_house_part varchar,
                                  addr_house_part_uuid varchar,
                                  addr_floor varchar,
                                  addr_floor_uuid varchar,
                                  addr_apartment varchar,
                                  addr_apartment_uuid varchar,


                                  message_description varchar,
                                  addr_additional_info varchar,

                                  msg_date timestamp,
                                  addressed_contact varchar,
                                  addressed_fio varchar,

                                  external_sys_name varchar
                                 );



create or replace function getMessageCard(int8) returns setof h_get_message_card as
$BODY$
declare
    message_id alias for $1;

    message_uuid varchar;
    r h_get_message_card%rowtype;

begin

    select uuid_t::varchar into message_uuid from q_base_table where id = message_id;

    if(message_uuid isnull) then
        return;
    end if;

    for r in select * from getMessageCard(message_uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$ language 'plpgsql';


create or replace function getMessageCard(varchar) returns setof h_get_message_card as
$BODY$
declare
    message_uuid alias for $1;

    r h_get_message_card%rowtype;

    tableName varchar;
    sql varchar;
begin

    if(message_uuid isnull) then
        return;
    end if;

    tableName = getRecordTableByUuid(message_uuid);
    if(tableName = 'eio_table_436') then
        sql = 'select * from getMessageCardAboutHPost(' || asString(message_uuid, true) || ')';
    elsif (tableName = 'eio_table_438') then
        sql = 'select * from getMessageCardAboutModeling(' || asString(message_uuid, true) || ')';
    elsif(tableName = 'eio_table_439') then
        sql = 'select * from getMessageCardAboutCamera(' || asString(message_uuid, true) || ')';
    elsif(tableName = 'eio_table_492') then
        sql = 'select * from getMessageCardAboutFire(' || asString(message_uuid, true) || ')';
    elsif(tableName = 'fiks_incidents_1') then
        sql = 'select * from getMessageCardAboutPortal(' || asString(message_uuid, true) || ')';
    else
        return;
    end if;


    for r in execute sql
    loop
        return next r;
    end loop;

    return;

end
$BODY$ language 'plpgsql';


create or replace function getMessageCardAboutModeling(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_438 where id = id_v;
    
    for r in select * from getMessageCardAboutModeling(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutModeling(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getMessageCardAboutModeling(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutModeling(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    hPostId int8;
    argusId int8;
    
    rr record; --h_get_message_card%rowtype;
    r h_get_message_card%rowtype;
begin

    hPostId = NULL;
    argusId = NULL;
    
    for rr in select event_id, argus_incident from eio_table_438 where uuid_t = externalBusUUID
    loop
        hPostId = rr.event_id;
        argusId = rr.argus_incident;
    end loop;

    if(hPostId is not null) then
        for r in select * from getMessageCardAboutModelingHPost(externalBusUUID)
        loop
            return next r;
        end loop;
    else
        for r in select * from getMessageCardAboutModelingArgus(externalBusUUID)
        loop
            return next r;
        end loop;
    end if;
    
    return;
end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutModelingHPost(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    r h_get_message_card%rowtype;
begin

    for r in 

    select 
        '8de44fac-ea70-11e4-8339-7054d244cccc' as bus_uuid, --m.uuid_t::varchar as bus_uuid, --������������� ������� ����� � ����������� ��������� (422)
        it.uuid_t as type_uuid, -- ��� ��
        it.name as type_name,

        --���������� �������
        ustr.latitude as latitude,
        ustr.longitude as longitude,
        NULL as altitude,

        --����� �������
        ustr.region_str_fias as addr_reg_name,
        NULL --addr1.uuid_t 
            as addr_reg_uuid,

        ustr.city_str_fias as addr_city_name,
        NULL -- addr2.uuid_t 
            as addr_city_uuid,

        ustr.street_str_fias as addr_street_name,
        NULL -- addr3.uuid_t 
            as addr_street_uuid,

        ustr.corps_str
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        ustr.house_number_str as addr_house_number,
        NULL -- addr5.uuid_t 
            as addr_house_uuid,

        ustr.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        ustr.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        ustr.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        NULL as message_description,
        ustr.param as addr_additional_info,

        m.dt as msg_date,

        NULL as addressed_contact,
        NULL as addressed_fio,

        u.code_name as external_sys_name
        
    from
        eio_table_438 m -- ���������� ����������� ������� �������������
        left join eio_table_436 vi on (m.event_id = vi.id) -- ���������� ������� ����������� �����
        --left join eio_table_424 it on (vi.incident = it.id) -- ������������� ������� ��������� �����������
        left join reestr_chs it on (m.type_chs3 = it.id) -- ������������� ����� ��
        left join eio_table_422 ustr on (vi.device = ustr.id) -- ���������� ��������� (Deprecated)

        --left join eio_table_391 addr1 on (ustr.region = addr1.id)
        --left join eio_table_391 addr2 on (ustr.sity = addr2.id)
        --left join eio_table_391 addr3 on (ustr.street = addr3.id)
        --left join eio_table_391 addr5 on (ustr.house = addr5.id)

        left join "position" pos on (m."medical-attributes-1198" = pos.id)
        left join units u on (pos.id_unit = u.id)


    where
        m.uuid_t = externalBusUUID

    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

create or replace function getMessageCardAboutModelingArgus(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    r h_get_message_card%rowtype;
begin

    for r in 

    select 
        '8de44fac-ea70-11e4-8339-7054d244cccc' as bus_uuid, --m.uuid_t::varchar as bus_uuid, --������������� ������� ����� � ����������� ��������� (422)
        it.uuid_t as type_uuid, -- ��� ��
        it.name as type_name,

        --���������� �������
        ustr.latitude as latitude,
        ustr.longitude as longitude,
        NULL as altitude,

        --����� �������
        ustr.region_str_fias as addr_reg_name,
        NULL --addr1.uuid_t 
            as addr_reg_uuid,

        ustr.city_str_fias as addr_city_name,
        NULL -- addr2.uuid_t 
            as addr_city_uuid,

        ustr.street_str_fias as addr_street_name,
        NULL -- addr3.uuid_t 
            as addr_street_uuid,

        ustr.corps_str
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        ustr.house_number_str as addr_house_number,
        NULL -- addr5.uuid_t 
            as addr_house_uuid,

        ustr.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        ustr.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        ustr.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        NULL as message_description,
        ustr.param as addr_additional_info,

        m.dt as msg_date,

        NULL as addressed_contact,
        NULL as addressed_fio,

        u.code_name as external_sys_name
        
    from
        eio_table_438 m -- ���������� ����������� ������� �������������
        left join eio_table_492 vi on (m.argus_incident = vi.id) -- ���������� ������� �������������� �����-������
        --left join eio_table_424 it on (vi.incident = it.id) -- ������������� ������� ��������� �����������
        left join reestr_chs it on (m.type_chs3 = it.id) -- ������������� ����� ��
        left join eio_table_422 ustr on (vi.device = ustr.id) -- ���������� ��������� (Deprecated)

        --left join eio_table_391 addr1 on (ustr.region = addr1.id)
        --left join eio_table_391 addr2 on (ustr.sity = addr2.id)
        --left join eio_table_391 addr3 on (ustr.street = addr3.id)
        --left join eio_table_391 addr5 on (ustr.house = addr5.id)

        left join "position" pos on (m."medical-attributes-1198" = pos.id)
        left join units u on (pos.id_unit = u.id)


    where
        m.uuid_t = externalBusUUID

    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

/* ******************************************** */

create or replace function getMessageCardAboutHPost(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_436 where id = id_v;
    
    for r in select * from getMessageCardAboutHPost(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutHPost(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getMessageCardAboutHPost(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getMessageCardAboutHPost(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    r h_get_message_card%rowtype;
begin

    for r in 

    select 
        ustr.uuid_t::varchar as bus_uuid, --������������� ����������
        it.uuid_t as type_uuid,
        it.incident_name as type_name,

        --���������� �������
        ustr.latitude as latitude,
        ustr.longitude as longitude,
        NULL as altitude,

        --����� �������
        ustr.region_str_fias as addr_reg_name,
        NULL --addr1.uuid_t 
            as addr_reg_uuid,

        ustr.city_str_fias as addr_city_name,
        NULL -- addr2.uuid_t 
            as addr_city_uuid,

        ustr.street_str_fias as addr_street_name,
        NULL -- addr3.uuid_t 
            as addr_street_uuid,

        ustr.corps_str
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        ustr.house_number_str as addr_house_number,
        NULL -- addr5.uuid_t 
            as addr_house_uuid,

        ustr.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        ustr.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        ustr.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        NULL as message_description,
        ustr.param as addr_additional_info,

        vi.dt as msg_date,

        NULL as addressed_contact,
        NULL as addressed_fio,

        u.code_name as external_sys_name
        
    from
        eio_table_436 vi -- ���������� ������� ����������� �����
        left join eio_table_424 it on (vi.incident = it.id) -- ������������� ������� ��������� �����������
        left join eio_table_422 ustr on (vi.device = ustr.id) -- ���������� ��������� (Deprecated)

        --left join eio_table_391 addr1 on (ustr.region = addr1.id)
        --left join eio_table_391 addr2 on (ustr.sity = addr2.id)
        --left join eio_table_391 addr3 on (ustr.street = addr3.id)
        --left join eio_table_391 addr5 on (ustr.house = addr5.id)

        left join "position" pos on (vi."medical-attributes-1198" = pos.id)
        left join units u on (pos.id_unit = u.id)


        --eio_table_424 it, -- ������������� ������� ��������� �����������
        --eio_table_422 ustr, -- ���������� ��������� (Deprecated)
        
        --eio_table_346 addr1, -- ���������� �������
        --eio_table_346 addr2, -- ���������� �������
        --eio_table_346 addr3, -- ���������� �������
        --eio_table_346 addr4, -- ���������� �������
        
        --"position" pos, --���������� ����������
       -- units u --���������� ������������� (���������)

    where
        vi.uuid_t = externalBusUUID

    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';


/* ******************************************** */

create or replace function getMessageCardAboutFire(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_492 where id = id_v;
    
    for r in select * from getMessageCardAboutFire(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutFire(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getMessageCardAboutFire(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function getMessageCardAboutFire(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    r h_get_message_card%rowtype;
begin

    for r in 

    select 
        ustr.uuid_t::varchar as bus_uuid, --������������� ����������
        it.uuid_t as type_uuid,
        it.incident_name as type_name,

        --���������� �������
        ustr.latitude as latitude,
        ustr.longitude as longitude,
        NULL as altitude,

        --����� �������
        ustr.region_str_fias as addr_reg_name,
        NULL --addr1.uuid_t 
            as addr_reg_uuid,

        ustr.city_str_fias as addr_city_name,
        NULL -- addr2.uuid_t 
            as addr_city_uuid,

        ustr.street_str_fias as addr_street_name,
        NULL -- addr3.uuid_t 
            as addr_street_uuid,

        ustr.corps_str
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        ustr.house_number_str as addr_house_number,
        NULL -- addr5.uuid_t 
            as addr_house_uuid,

        ustr.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        ustr.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        ustr.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        NULL as message_description,
        ustr.param as addr_additional_info,

        vi.dt as msg_date,

        NULL as addressed_contact,
        NULL as addressed_fio,

        u.code_name as external_sys_name
        
    from
        eio_table_492 vi -- ���������� ������� �������� ������
        left join eio_table_424 it on (vi.incident = it.id) -- ������������� ������� ��������� �����������
        left join eio_table_422 ustr on (vi.device = ustr.id) -- ���������� ��������� (Deprecated)

        --left join eio_table_391 addr1 on (ustr.region = addr1.id)
        --left join eio_table_391 addr2 on (ustr.sity = addr2.id)
        --left join eio_table_391 addr3 on (ustr.street = addr3.id)
        --left join eio_table_391 addr5 on (ustr.house = addr5.id)

        left join "position" pos on (vi."medical-attributes-1198" = pos.id)
        left join units u on (pos.id_unit = u.id)
  
    where
        vi.uuid_t = externalBusUUID

    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';


/* **************************************/






create or replace function getMessageCardAboutCamera(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from eio_table_439 where id = id_v;
    
    for r in select * from getMessageCardAboutCamera(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutCamera(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getMessageCardAboutCamera(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';



create or replace function getMessageCardAboutCamera(uuid) returns setof h_get_message_card as 
$BODY$
declare
    externalBusUUID alias for $1;

    r h_get_message_card%rowtype;
begin

    for r in 

    select 
        ustr.uuid_t::varchar as bus_uuid, --������������� ����������
        it.uuid_t as type_uuid,
        it.incident_name as type_name,

        --���������� �������
        ustr.latitude as latitude,
        ustr.longitude as longitude,
        NULL as altitude,

        --����� �������
        ustr.region_str_fias as addr_reg_name,
        NULL --addr1.uuid_t 
            as addr_reg_uuid,

        ustr.city_str_fias as addr_city_name,
        NULL -- addr2.uuid_t 
            as addr_city_uuid,

        ustr.street_str_fias as addr_street_name,
        NULL -- addr3.uuid_t 
            as addr_street_uuid,

        ustr.corps_str
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        ustr.house_number_str as addr_house_number,
        NULL -- addr5.uuid_t 
            as addr_house_uuid,

        ustr.house_part_str
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        ustr.floor_str
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        ustr.apartment_str
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        NULL as message_description,
        ustr.param as addr_additional_info,

        vi.dt as msg_date,

        NULL as addressed_contact,
        NULL as addressed_fio,

        u.code_name as external_sys_name
        
    from
        eio_table_439 vi -- ���������� ������� �������������� ����� ��������������
        left join eio_table_424 it on (vi.incident = it.id)
        left join eio_table_422 ustr on (vi.device = ustr.id)

        --left join eio_table_391 addr1 on (ustr.region = addr1.id)
        --left join eio_table_391 addr2 on (ustr.sity = addr2.id)
        --left join eio_table_391 addr3 on (ustr.street = addr3.id)
        --left join eio_table_391 addr5 on (ustr.house = addr5.id)

        left join "position" pos on (vi.position = pos.id)
        left join units u on (pos.id_unit = u.id)


        --eio_table_424 it, -- ������������� ������� ��������� �����������
        --eio_table_422 ustr, -- ���������� ��������� (Deprecated)
        
        --eio_table_346 addr1, -- ���������� �������
        --eio_table_346 addr2, -- ���������� �������
        --eio_table_346 addr3, -- ���������� �������
        --eio_table_346 addr4, -- ���������� �������
        
        --"position" pos, --���������� ����������
       -- units u --���������� ������������� (���������)

    where
        vi.uuid_t = externalBusUUID
        --vi.id = 918
        --and vi.incident = it.id
        --and vi.device = ustr.id
        --and ustr.region = addr1.id
        --and ustr.sity = addr2.id
        --and ustr.street = addr3.id
        --and ustr.house = addr4.id
        --and vi.position = pos.id
        --and pos.id_unit = u.id

    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';

/* *************************************/


create or replace function getMessageCardAboutPortal(int8) returns setof h_get_message_card as 
$BODY$
declare
    id_v alias for $1;
    r h_get_message_card%rowtype;
    uuid_v uuid;
begin

    select uuid_t into uuid_v from fiks_incidents_1 where id = id_v;
    
    for r in select * from getMessageCardAboutPortal(uuid_v)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';


create or replace function getMessageCardAboutPortal(varchar) returns setof h_get_message_card as 
$BODY$
declare
    uuid_v alias for $1;
    r h_get_message_card%rowtype;
begin

    for r in select * from getMessageCardAboutPortal(uuid_v::uuid)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

--select * from fiks_incidents_1
create or replace function getMessageCardAboutPortal(uuid) returns setof h_get_message_card as
$BODY$
declare

    externalBusUUID alias for $1;
    
    r h_get_message_card%rowtype;
begin


    for r in 
    select 
        fiks.uuid_t::varchar as bus_uuid,
        it.uuid_t as type_uuid,
        it.name as type_name, -- ��� ��

        --���������� �������
        fiks.latitude as latitude,
        fiks.longitude as longitude,
        NULL as altitude,

        --����� �������
        addr1.formalname as addr_reg_name,
        addr1.uuid_t as addr_reg_uuid,

        addr2.formalname as addr_city_name,
        addr2.uuid_t as addr_city_uuid,

        addr3.formalname as addr_street_name,
        addr3.uuid_t as addr_street_uuid,

        NULL -- addr4.formalname 
            as addr_house_number_add,
        NULL --addr4.uuid_t 
            as addr_house_number_add_uuid,

        addr5.formalname as addr_house_number,
        addr5.uuid_t as addr_house_uuid,

        NULL --addr6.formalname 
            as addr_house_part,
        NULL --addr6.uuid_t 
            as addr_house_part_uuid,

        NULL -- addr7.formalname 
            as addr_floor,
        NULL -- addr7.uuid_t 
            as addr_floor_uuid,

        NULL --addr8.formalname 
            as addr_apartment,
        NULL --addr8.uuid_t 
            as addr_apartment_uuid,

        fiks.annotation as message_description,
        fiks.dop_inf as addr_additional_info,

        fiks.data_time_chs as msg_date,

        fiks.kontact as addressed_contact,
        fiks.fio as addressed_fio,

        u.code_name as external_sys_name


        --'' as external_sys_name
        
    from
        fiks_incidents_1 fiks -- ���������� ������� �������� �������
        --left join eio_table_424 it on (fiks.incident = it.id) -- ������������� ������� ��������� �����������
        left join reestr_chs it on (fiks.type_chs3 = it.id) --��� ��

        left join eio_table_391 addr1 on (fiks.region = addr1.id)
        left join eio_table_391 addr2 on (fiks.sity = addr2.id)
        left join eio_table_391 addr3 on (fiks.street = addr3.id)
        left join eio_table_391 addr5 on (fiks.house = addr5.id)

        left join "position" pos on (fiks.position = pos.id)
        left join units u on (pos.id_unit = u.id)

    where
        fiks.uuid_t = externalBusUUID

    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

--select * from tbl_reestr_chs;
