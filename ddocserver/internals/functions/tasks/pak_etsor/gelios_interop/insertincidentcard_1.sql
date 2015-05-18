
/*

select * from io_objects where id = 490
select * from io_categories where id = 487
select * from attributes where id in (select id_io_attribute from attrs_categories where id_io_category = 486);


select createTempTables();
select setCurrentDl(4);

assotiated_video;�������������� ������� ��������������
assotiated_hpost;��������������� ������� �����������
assotiated_modeling;��������������� ������� �������������



��������� ������� �������� (���������) �������� ������������.
���� �� ����������� ������ � additionalInfoList. ������ ���� ����������
�� ����������� ����� ��������� ��������. ���� ����������

select * from eio_table_490;
create or replace function insertIncidentCard(varchar, --UUID _������������_(!!). ��� �������� ������ ���� ���� (NULL). ���� �� NULL ����� ��������� ��������
                                              varchar, -- ��� ������������. �� �������������� 424-��. ���� ���������� ������ �������������, ��� �� ���� ��� ��������
                                              float8, --����������
                                              float8, 
                                              float8, 

                                              --�����
                                              varchar, -- ������ (�����)
                                              varchar, --����� (�����)
                                              varchar, --����� (�����)
                                              varchar, --���. ������ � ������ ����  (�����)
                                              varchar, -- ����� ����  (�����)
                                              varchar,-- �������� (�����)
                                              varchar,-- ����  (�����)
                                              varchar,--����� ��������  (�����)

                                              varchar,--�������� ������������
                                              varchar,--�������������� ���������� �� ������

                                              timestamp with time zone, --����-����� ������������
                                              varchar, -- �������� �������������
                                              varchar, --��� �������������
                                              varchar, --����� GELIOS (!!! � ������� ������)
                                              varchar[] --���������� �������������� �������, ��������������� � �������������
                                              ) returns varchar as

������:
select insertIncidentCard_1( NULL, --(select uuid_t::varchar from eio_table_490 where id = 2114), 
                          (select uuid_t::varchar from eio_table_424 where id = 2017),
                          25.0::float8, 33.0::float8, 0.0::float8,
                          'new region',
                          'new city',
                          'new street',
                          'new corps',
                          'new house',
                          'new ��������',
                          'new floor',
                          'new apartment',

                          '����� ������������'::varchar,
                          NULL::varchar,
                          now(),
                          'addressed_contact'::varchar, 'fio'::varchar, 'GELIOS'::varchar, 
                          (select array_agg(mess_uuid::varchar) from getIncidentMessages(2082))); 
*/

create or replace function insertIncidentCard_1(varchar, 
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
    
    addr_reg_str  alias for $6;
    addr_city_str  alias for $7;
    addr_street_str  alias for $8;
    addr_house_number_add_str  alias for $9;
    addr_house_str  alias for $10;
    addr_house_part_str  alias for $11;
    addr_floor_str  alias for $12;
    addr_apartment_str  alias for $13;

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
        return updateIncidentCard_1(bus_uuid,
                                  type_uuid,
                                  lat,
                                  lon,
                                  altitude,
                                  
                                  addr_reg_str,
                                  addr_city_str,
                                  addr_street_str,
                                  addr_house_number_add_str,
                                  addr_house_str,
                                  addr_house_part_str,
                                  addr_floor_str,
                                  addr_apartment_str,

                                  message_description,
                                  addr_additional_info,

                                  msg_date,
                                  addressed_contact,
                                  addressed_fio,
                                  
                                  external_sys_name,
                                  message_uuids);
    end if;


    --select id into id_region from eio_table_391 where uuid_t = addr_reg_uuid::uuid;
    --select id into id_city from eio_table_391 where uuid_t = addr_city_uuid::uuid;
    --select id into id_street  from eio_table_391 where uuid_t = addr_street_uuid::uuid;
    --select id into id_house from eio_table_391 where uuid_t = addr_house_uuid::uuid;
    select id into id_type from eio_table_424 where uuid_t = type_uuid::uuid;

    select getNextSeq('eio_table_490', 'id') into idCard;

    insert into eio_table_490 (id, 
                               latitude,
                               longitude,
                               
                               region_str,
                               city_str,
                               street_str,
                               corps_str,
                               house_number_str,
                               house_part_str,
                               floor_str,
                               apartment_str,

                               dop_inf,

                               annotation,
                               
                               data_time_kart,  
                               data_time_in_data,
                               data_time_chs,
                     
                               kontact, --�������� �������������
                               fio, --��� ������������� � (���) �������� �������.
                               
                               type_chs3, --��� ��
                               incident,
                               guuid_chs3) --������������ ������������(��)
                         values(idCard, 
                                lat, lon, 

                                addr_reg_str, addr_city_str, addr_street_str, addr_house_number_add_str, addr_house_str, addr_house_part_str, addr_floor_str, addr_apartment_str,
                                addr_additional_info, 

                                message_description, 
                                msg_date, msg_date, msg_date, 

                                addressed_contact, addressed_fio, 
                                
                                96, --��� ��. ����� ���. ���� �� ����������. ����������� �������� �� ���������
                                id_type, --��� �� (������)
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

create or replace function updateIncidentCard_1(varchar, 
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
    
    addr_reg_str  alias for $6;
    addr_city_str  alias for $7;
    addr_street_str  alias for $8;
    addr_house_number_add_str  alias for $9;
    addr_house_str  alias for $10;
    addr_house_part_str  alias for $11;
    addr_floor_str  alias for $12;
    addr_apartment_str  alias for $13;

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
        return insertIncidentCard_1(bus_uuid,
                                  type_uuid,
                                  lat,
                                  lon,
                                  altitude,
                                  
                                  addr_reg_str,
                                  addr_city_str,
                                  addr_street_str,
                                  addr_house_number_add_str,
                                  addr_house_str,
                                  addr_house_part_str,
                                  addr_floor_str,
                                  addr_apartment_str,

                                  message_description,
                                  addr_additional_info,

                                  msg_date,
                                  addressed_contact,
                                  addressed_fio,
                                  
                                  external_sys_name,
                                  message_uuids);
    end if;


--    select id into id_region from eio_table_391 where uuid_t = addr_reg_uuid::uuid;
--    select id into id_city from eio_table_391 where uuid_t = addr_city_uuid::uuid;
--    select id into id_street  from eio_table_391 where uuid_t = addr_street_uuid::uuid;
--    select id into id_house from eio_table_391 where uuid_t = addr_house_uuid::uuid;
    select id into id_type from eio_table_424 where uuid_t = type_uuid::uuid;


    update eio_table_490 set
    
        latitude = lat,
        longitude = lon,

        region_str = addr_reg_str,
        city_str = addr_city_str,
        street_str = addr_street_str,
        corps_str = addr_house_number_add_str,
        house_number_str = addr_house_str,
        house_part_str = addr_house_part_str,
        floor_str = addr_floor_str,
        apartment_str = addr_apartment_str,

        dop_inf = addr_additional_info,

        annotation = message_description,

        data_time_kart = msg_date,  
        data_time_in_data = msg_date,
        data_time_chs = msg_date,
        
        kontact = addressed_contact, --�������� �������������
        fio = addressed_fio, --��� ������������� � (���) �������� �������.
                               
        type_chs3 = 96, --��� ��
        incident = id_type,
        guuid_chs3 = type_uuid --������������ ������������(��)
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
