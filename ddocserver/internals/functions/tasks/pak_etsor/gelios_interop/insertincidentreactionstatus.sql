
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


create or replace function insertIncidentCard(varchar, --UUID _������������_(!!). ��� �������� ������ ���� ���� (NULL). ���� �� NULL ����� ��������� ��������
                                              varchar, -- ��� ������������. �� �������������� 424-��. ���� ���������� ������ �������������, ��� �� ���� ��� ��������
                                              float8, --����������
                                              float8, 
                                              float8, 

                                              --�����
                                              varchar, -- ��� �������
                                              varchar, --��� ������
                                              varchar, --��� �����
                                              varchar, --��� ���. ������ � ������ ���� (�� ������������ ����, �������� NULL)
                                              varchar, --��� ������ ���� 
                                              varchar,-- ��� �������� (�� ������������ ����, �������� NULL)
                                              varchar,-- ��� ����� (�� ������������ ����, �������� NULL)
                                              varchar,--��� ������ �������� (�� ������������ ����, �������� NULL)

                                              varchar,--�������� ������������
                                              varchar,--�������������� ���������� �� ������

                                              timestamp with time zone, --����-����� ������������
                                              varchar, -- �������� �������������
                                              varchar, --��� �������������
                                              varchar, --����� GELIOS (!!! � ������� ������)
                                              varchar[] --���������� �������������� �������, ��������������� � �������������
                                              ) returns varchar as

������:
select insertIncidentReactionStatus( NULL, --(select uuid_t::varchar from incident_reaction where id = 2875), 
                                    (select uuid_t::varchar from eio_table_490 where id = 1971),
                          
                                    'asasas',
                                    now(),
                                    
                                    (select uuid_t::varchar from eio_table_352 where id = 341), -- statusType (������ ������)
                                    NULL::varchar,
                                    
                                    (select uuid_t::varchar from kl_chs where id = 2803), -- id_dds (��� 01)
                                    NULL::varchar, 
                                    NULL::varchar
                                  ); 

select * from incident_reaction
select * from incident_reaction_kl_chs_ref_1322
delete from incident_reaction_kl_chs_ref_1322 where id_kl_chs = 2803
*/

create or replace function insertIncidentReactionStatus(varchar, 
                                                        varchar, 
                                                        varchar,
                                                        timestamp with time zone,
                                                        varchar,
                                                        varchar,
                                                        varchar,
                                                        varchar,
                                                        varchar) returns varchar as
$BODY$
declare

    bus_uuid alias for $1; --UUID ������� ������������ (NULL, ���� ��������� �����)
    incident_uuid alias for $2;--UUID ������������
    
    add_info alias for $3;

    reactionDatetime alias for $4;
    reactionTypeUuid alias for $5;
    reactionStatusName alias for $6;

    dds_uuid alias for $7;
    dds_type_name alias for $8;
    dds_unit_name alias for $9;
    

    incident_reaction_uuid varchar;
    idIncidentReaction int8;
    idIncident int8;
    idTypeReaction int8;

    idDds int8;
    cnt int4;

begin

    if(bus_uuid is not null) then
        return updateIncidentReactionStatus(bus_uuid,
                                            incident_uuid,
                                            add_info,
                                            
                                            reactionDatetime,
                                            reactionTypeUuid,
                                            reactionStatusName,
                                            
                                            dds_uuid,
                                            dds_type_name,
                                            dds_unit_name);
    end if;

--select * from incident_reaction

    select id into idIncident from eio_table_490 where uuid_t = incident_uuid::uuid; --������������
    select id into idTypeReaction from eio_table_352 where uuid_t = reactionTypeUuid::uuid; --��� ������� ������������
    select id into idDds from kl_chs where uuid_t = dds_uuid::uuid; --������ ������������
    
    select getNextSeq('incident_reaction', 'id') into idIncidentReaction;

    insert into incident_reaction (id, 
                                   description,
                                   reaction_datetime, 
                                   reaction_status,
                                   incident_id
                                  )
                         values(idIncidentReaction, 
                                add_info,
                                reactionDatetime,
                                idTypeReaction,
                                idIncident
                               );


    select uuid_t::varchar into incident_reaction_uuid from incident_reaction where id = idIncidentReaction;

    if(incident_reaction_uuid is not null) then

        select count(*) into cnt from incident_reaction_kl_chs_ref_1322 where id_incident_reaction = idIncidentReaction and id_kl_chs = idDds;
        if(cnt = 0) then
            insert into incident_reaction_kl_chs_ref_1322 (id_incident_reaction, id_kl_chs) 
            values (idIncidentReaction, idDds);
        end if;
         
    end if;

    return incident_reaction_uuid;
end
$BODY$
language 'plpgsql';

create or replace function updateIncidentReactionStatus(varchar, 
                                                        varchar, 
                                                        varchar,
                                                        timestamp with time zone,
                                                        varchar,
                                                        varchar,
                                                        varchar,
                                                        varchar,
                                                        varchar) returns varchar as
$BODY$
declare

    bus_uuid alias for $1; --UUID ������� ������������ (NULL, ���� ��������� �����)
    incident_uuid alias for $2;--UUID ������������
    
    add_info alias for $3;

    reactionDatetime alias for $4;
    reactionTypeUuid alias for $5;
    reactionStatusName alias for $6;

    dds_uuid alias for $7;
    dds_type_name alias for $8;
    dds_unit_name alias for $9;


    idIncidentReaction int8;
    idIncident int8;
    idTypeReaction int8;

    idDds int8;
    cnt int4;

begin


    if(bus_uuid is null) then
        return updateIncidentReactionStatus(bus_uuid,
                                            incident_uuid,
                                            add_info,
                                            
                                            reactionDatetime,
                                            reactionTypeUuid,
                                            reactionStatusName,
                                            
                                            dds_uuid,
                                            dds_type_name,
                                            dds_unit_name);
    end if;


    select id into idIncident from eio_table_490 where uuid_t = incident_uuid::uuid; --������������
    select id into idTypeReaction from eio_table_352 where uuid_t = reactionTypeUuid::uuid; --��� ������� ������������
    select id into idDds from kl_chs where uuid_t = dds_uuid::uuid; --������ ������������


    update incident_reaction set
        description = add_info,
        reaction_datetime = reactionDatetime, 
        reaction_status = idTypeReaction,
        incident_id = idIncident
     where 
         uuid_t = bus_uuid::uuid;

    select id into idIncidentReaction from incident_reaction where uuid_t = bus_uuid::uuid;

    if(idIncidentReaction is not null) then

        select count(*) into cnt from incident_reaction_kl_chs_ref_1322 where id_incident_reaction = idIncidentReaction and id_kl_chs = idDds;
        if(cnt = 0) then
            insert into incident_reaction_kl_chs_ref_1322 (id_incident_reaction, id_kl_chs) 
            values (idIncidentReaction, idDds);
        end if; 
    end if;


    return bus_uuid;
end
$BODY$
language 'plpgsql';
