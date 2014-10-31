select f_safe_drop_type('h_get_out_xml_messages');
create type h_get_out_xml_messages as (full_address varchar,
                                       id int8,
                                       id_format int4,
                                       id_organization int4,
                                       interaction_type int4,
                                       out_data varchar,
                                       port int4,
                                       org_uid varchar, --email_prefix
                                       use_gateway bool
                                      );

create or replace function getOutXMLMessages() returns setof h_get_out_xml_messages as
$BODY$
declare
    r h_get_out_xml_messages%rowtype;
    idTransport int4;
    processedIds int8[];
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    processedIds = ARRAY[]::int8[];

    for r in 
        select 
            (select address from uGetAddressExOrg(q.id_organization, idTransport)) as full_address,
            q.id, 
            q.id_format, 
            q.id_organization, 
            q.interaction_type, 
            q.out_data,
            (select port from uGetAddressExOrg(q.id_organization, idTransport)) as port,
            (select org_uid from uGetAddressExOrg(q.id_organization, idTransport)) as org_uid,
            (select use_gateway from uGetAddressExOrg(q.id_organization, idTransport)) as use_gateway
        from out_external_queue q
        where 
            q.interaction_result = 1
            and getLocalOrgId() <> q.id_organization
        limit 100 --будем брать данные порциями не более чем по 100 штук, чтобы не перенагружать клиента
    loop
        if(r.full_address is not null) then
            processedIds = processedIds || r.id;
            return next r;
        end if;
    end loop;

    update out_external_queue set interaction_result = 3 where id = ANY (processedIds); --set all received messages as transferred OK! maybe need some more steps...

    return;
end
$BODY$
language 'plpgsql';
