create or replace function insertOrganization(varchar, varchar, varchar, varchar, int4, int4) returns int4 as
$BODY$
declare
    oName alias for $1;
    oShortName alias for $2;
    oCodeName alias for $3;
    oEmailPrefix alias for $4;
    oType alias for $5;
    oParent alias for $6;

    pix1 varchar;
    pix2 varchar;
    idOrg int4;

begin

    select def_value into pix1 from attrs_categories where id_io_category = 53 and id_io_attribute = 78;
    select def_value into pix2 from attrs_categories where id_io_category = 53 and id_io_attribute = 90;

    select getNextSeq('organization', 'id') into idOrg;
    insert into organization (id, id_type_org, id_parent, name, short_name, code_name, email_prefix, latitude, longitude, map_symbol, tree_symbol) values
                             (idOrg,
                              oType, 
                              oParent,
                              oName,
                              oShortName,
                              oCodeName,
                              oEmailPrefix,
                              0,
                              0,
                              pix1,
                              pix2);

    return idOrg;


end
$BODY$
language 'plpgsql';

--in information exchange
create or replace function insertOrganizationEx(varchar, int4, varchar, varchar, varchar, int4, int4, varchar, varchar, varchar, varchar, float8, float8, timestamp, timestamp, varchar, varchar, int4[]) returns int4 as
$BODY$
declare
    uidOrg alias for $1;
    idType alias for $2;
    uidParent alias for $3;
    uidParent1 alias for $4;
    uidSubstitutor alias for $5;
    idCurrMode alias for $6;
    idPrevMode alias for $7;
    oName alias for $8;
    oShortName alias for $9;
    oCodeName alias for $10;
    oEmailPrefix alias for $11;
    oLat alias for $12;
    oLon alias for $13;
    dtCurrMode alias for $14;
    dtPrevMode alias for $15;
    pxMapSymbol alias for $16;
    pxTreeSymbol alias for $17;
    wmModes alias for $18;
    
    idOrg int4;
    idParent int4;
    idParent1 int4;
    idSubstitutor int4;
    pix1 varchar;
    pix2 varchar;

begin

    if(getCurrentUser() <> 2) then --'jupiter'
        return -1;
    end if;

    pix1 := pxMapSymbol;
    pix2 := pxTreeSymbol;
  
    if(pxMapSymbol isnull) then
        select def_value into pix1 from attrs_categories where id_io_category = 53 and id_io_attribute = 78;
    end if;
    if(pxTreeSymbol isnull) then
        select def_value into pix2 from attrs_categories where id_io_category = 53 and id_io_attribute = 90;
    end if;

    select id into idOrg from organization where email_prefix = oEmailPrefix;
    if(idOrg is not null) then
        /*
        update organization set
                            id_type_org = idType,
                            id_parent = idParent,
                            id_parent1 = idParent1,
                            id_substitutor = idSubstitutor,
                            id_curr_mode = idCurrMode,
                            id_prev_mode = idPrevMode,

                            name = oName,
                            short_name = oShortName,
                            code_name = oCodeName, 
                            latitude = oLat, 
                            longitude = oLon, 
                            map_symbol = pix1, 
                            tree_symbol = pix2,
                            dt_prev_mode = dtCurrMode,
                            dt_curr_mode  = dtPrevMode
        where
            id = idOrg;

        delete from organization_work_mode where id_organization = idOrg;
        insert into organization_work_mode (id_organization, id_work_mode) select idOrg, unnest(wmModes);
        */
        
        return idOrg;
    end if;

    select getNextSeq('organization', 'id') into idOrg;
    insert into organization (id, 
                              id_type_org, 
                              id_parent, 
                              id_parent1, 
                              id_substitutor,
                              id_curr_mode,
                              id_prev_mode,
                              
                              name, 
                              short_name, 
                              code_name, 
                              email_prefix, 
                              latitude, 
                              longitude, 
                              map_symbol, 
                              tree_symbol,
                              dt_prev_mode,
                              dt_curr_mode) 
                      values
                             (idOrg,
                              idType,
                              idParent,
                              idParent1,
                              idSubstitutor,
                              idCurrMode,
                              idPrevMode,

                              oName,
                              oShortName,
                              oCodeName,
                              oEmailPrefix,
                              oLat,
                              oLon,
                              pix1,
                              pix2,
                              dtCurrMode,
                              dtPrevMode);

    delete from organization_work_mode where id_organization = idOrg;
    insert into organization_work_mode (id_organization, id_work_mode) select idOrg, unnest(wmModes);

    return idOrg;

end
$BODY$
language 'plpgsql';
