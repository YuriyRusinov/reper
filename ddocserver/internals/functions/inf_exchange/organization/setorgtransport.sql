create or replace function setOrganizationAddress (int4, int4[], varchar[], boolean[]) returns int4 as
$BODY$
declare
    idOrg alias for $1;
    idTransportList alias for $2;
    addressList alias for $3;
    activeList alias for $4;

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
        select into cnt count(*) from organization_transport ot where ot.id_organization=idOrg and ot.id_transport=idTransportList[i];
        if (cnt = 0 and length (trim(addressList[i])) > 0) then
            select getNextSeq('organization_transport', 'id') into id_org_tr;
            query := E'insert into organization_transport (id, id_organization, id_transport, address';
            if (activeList is not null and i <= array_upper (activeList, 1)) then
                query := query || E',is_active';
            end if;
            query := query || E') values (' || id_org_tr || E',' || idOrg || E',' || idTransportList[i] || E',' || quote_literal (addressList[i]);
            if (activeList is not null and i <= array_upper (activeList, 1)) then
                query := query || E',' || activeList[i];
            end if;
            query := query || E');';
        elsif (cnt > 0 and length (trim(addressList[i])) > 0) then
            select into id_org_tr id from organization_transport ot where ot.id_organization=idOrg and ot.id_transport=idTransportList[i];
            query := E'update organization_transport set address=' || quote_literal (addressList[i]);
            raise warning 'active is % % %', i, activeList[i], array_upper (activeList, 1);
            if (activeList is not null and i <= array_upper (activeList, 1)) then
                query := query || E', is_active=' || activeList[i];
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
