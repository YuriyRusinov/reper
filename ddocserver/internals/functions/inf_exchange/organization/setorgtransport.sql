create or replace function setOrganizationAddress (int4, int4[], varchar[], int4[], boolean[], boolean[]) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    idTransportList alias for $2;
    addressList alias for $3;
    portList alias for $4;
    activeList alias for $5;
    useGatewayList alias for $6;

    cnt int4;
    is_active boolean;
    ntr int4;
    i int4;
    id_org_tr int4;
    query varchar;
begin
    if (array_upper (idTransportList, 1) <> array_upper (addressList, 1) ) then
        raise warning 'Incomplete addresses/transporters list';
        return -1;
    end if;

    ntr := array_upper (idTransportList, 1);

    for i in 1..ntr
    loop
        query := NULL;
        id_org_tr := NULL;

        select into cnt count(*) 
        from organization_transport ot 
        where 
            ot.id_organization=idOrg 
            and ot.id_transport=idTransportList[i];

        if (cnt = 0 and length (trim(addressList[i])) > 0) then
            select getNextSeq('organization_transport', 'id') into id_org_tr;
            query := E'insert into organization_transport (id, id_organization, id_transport, address, port';

            if (activeList is not null and i <= array_upper (activeList, 1)) then
                query := query || E',is_active';
            end if;

            if (useGatewayList is not null and i <= array_upper (useGatewayList, 1)) then
                query := query || E', use_gateway';
            end if;

            query := query || E') values (' || id_org_tr || E',' || idOrg || E',' || idTransportList[i] || E',' || quote_literal (addressList[i]);

            if (portList is not null and i <= array_upper (portList, 1)) then
                if(portList[i] is not null) then
                    query := query || E',' || portList[i];
                else
                    query := query || E', NULL ';
                end if;
            end if;

            if (activeList is not null and i <= array_upper (activeList, 1)) then
                if(activeList[i] is not null) then
                    query := query || E',' || activeList[i];
                else
                    query := query || E', true ';
                end if;
            end if;

            if (useGatewayList is not null and i <= array_upper (useGatewayList, 1)) then
                if(useGatewayList[i] is not null) then
                    query := query || E',' || useGatewayList[i];
                else
                    query := query || E', false ';
                end if;
            end if;

            query := query || E');';

        elsif (cnt > 0 and length (trim(addressList[i])) > 0) then

            select into id_org_tr id from organization_transport ot where ot.id_organization=idOrg and ot.id_transport=idTransportList[i];

            query := E'update organization_transport set address = ' || quote_literal (addressList[i]);

            raise warning 'active is % % %', i, activeList[i], array_upper (activeList, 1);

            if (activeList is not null and i <= array_upper (activeList, 1)) then
                if(activeList[i] is not null) then
                    query := query || E', is_active=' || activeList[i];
                else
                    query := query || E', is_active = true';
                end if;
            end if;

            if (useGatewayList is not null and i <= array_upper (useGatewayList, 1)) then
                if(useGatewayList[i] is not null) then
                    query := query || E', use_gateway =' || useGatewayList[i];
                else
                    query := query || E', use_gateway = false';
                end if;
            end if;

            if (portList is not null and i <= array_upper (portList, 1)) then
                if(portList[i] is not null) then
                    query := query || E', port =' || portList[i];
                else
                    query := query || E', port = NULL';
                end if;
            end if;

            query := query || E' where id=' || id_org_tr;
        elsif (cnt = 0 and length (trim(addressList[i])) = 0) then
            continue;
        elsif (cnt > 0 and length (trim(addressList[i])) = 0) then
            select into id_org_tr id from organization_transport ot where ot.id_organization=idOrg and ot.id_transport=idTransportList[i];
            query := E'delete from organization_transport where id=' || id_org_tr;
        else
            continue;
        end if;
        if (query is not null) then
            raise warning 'query is %', query;
            execute query;
        end if;
    end loop;
    return 1;
    exception when others then
        raise warning 'exception';
        return -1;

end
$BODY$
language 'plpgsql';
