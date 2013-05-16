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
