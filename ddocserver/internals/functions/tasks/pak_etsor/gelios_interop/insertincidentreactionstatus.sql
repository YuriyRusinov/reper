
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
select insertIncidentReactionStatus( NULL, --(select uuid_t::varchar from incident_reaction where id = 2875), 
                                    (select uuid_t::varchar from eio_table_490 where id = 1971),
                          
                                    'asasas',
                                    now(),
                                    
                                    (select uuid_t::varchar from eio_table_352 where id = 341), -- statusType (выслан экипаж)
                                    NULL::varchar,
                                    
                                    (select uuid_t::varchar from kl_chs where id = 2803), -- id_dds (ДДС 01)
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

    bus_uuid alias for $1; --UUID статуса реагирования (NULL, если создается новый)
    incident_uuid alias for $2;--UUID происшествия
    
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

    select id into idIncident from eio_table_490 where uuid_t = incident_uuid::uuid; --происшествие
    select id into idTypeReaction from eio_table_352 where uuid_t = reactionTypeUuid::uuid; --тип статуса реагирования
    select id into idDds from kl_chs where uuid_t = dds_uuid::uuid; --служба реагирования
    
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

    bus_uuid alias for $1; --UUID статуса реагирования (NULL, если создается новый)
    incident_uuid alias for $2;--UUID происшествия
    
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


    select id into idIncident from eio_table_490 where uuid_t = incident_uuid::uuid; --происшествие
    select id into idTypeReaction from eio_table_352 where uuid_t = reactionTypeUuid::uuid; --тип статуса реагирования
    select id into idDds from kl_chs where uuid_t = dds_uuid::uuid; --служба реагирования


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
