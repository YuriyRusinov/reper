create or replace function fdocToXML(int4, int4, int4[]) returns xml as
$BODY$
declare
    idObject alias for $1;
    idMsg alias for $2;
    idAddrList alias for $3;

    idCategory int4;

    result xml;
    xml_str varchar;
    io_name varchar;
    r record;
    regNumber varchar;

    xml_msg_passport xml;
    xml_form_pars xml;
begin
    if (idObject is null) then
        return null;
    end if;
    select into idCategory id_io_category from io_objects where id=idObject;
    if (idCategory != 170) then
        raise warning 'Incorrect IO, category is % must be %', idCategory, 170;
        return null;
    end if;
    --result := xmlroot(xmlparse(document (E'<?xml version="1.1" encoding="UTF-8"?>\n\n<msg></msg>')), 
               --version '1.0', standalone no);

    select into regNumber unique_id from io_objects where id = idObject;

    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n<msg>';--</msg>';
    xml_str := xml_str || E'\n';
    select into xml_msg_passport createPassport (idObject, idMsg, idAddrList, 2);

    --raise warning 'passport is %', xml_msg_passport;
    xml_str := xml_str || xml_msg_passport::varchar;--xpath_string ('xml', xml_msg_passport);-- createMsgPassport (idObject, NULL));
    xml_str := xml_str || E'\n\t';
    xml_str := xml_str || E'<body>\n';
    xml_str := xml_str || E'<command_data>\n';
    xml_str := xml_str || E'</command_data>\n';
    xml_str := xml_str || E'\t\t<formalized_document_data>\n';
    xml_str := xml_str || E'\t\t\t<Information Echelon=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_echelon')
    loop
        if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Cdomain=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_cdomain')
    loop
        if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'>\n';
    xml_str := xml_str || E'\t\t\t\t<EmergencySituation IncidentDateTime=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_datetime')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' EmergencySituationType=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_type')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Area=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_area')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Region=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_region')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' MilitaryDistrict=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_district')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Town=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_town')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'>\n';
    xml_str := xml_str || E'\t\t\t\t\t<RadiationPollutionDocument>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<DocumentEntry>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_document_entry')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</DocumentEntry>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<ObjectLongTitle>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_obj_open_name')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</ObjectLongTitle>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<SecretLongTitle>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_obj_close_name')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</SecretLongTitle>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<Belonging>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<Bloc>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_block')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</Bloc>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<Country>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_country')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</Country>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<Department>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_department')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</Department>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<Company>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_company')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</Company>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t</Belonging>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<IncindentSituationalScheme>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_pict')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</IncindentSituationalScheme>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<RadiationPolution Authenticity=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_authenticity')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' InstrumentationEventTime=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_instrumentation_event_time')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' CDomain=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_cdomain1')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<Partial>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<IncindentShortDescription>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_desc')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</IncindentShortDescription>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<Coordinates HeightUnitOfMeasure=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_height_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Height=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_height')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Longitude=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_lon')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' CDomain=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_cdomain2')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Latitude=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_lat')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' CoordinatesUnitOfMeasure=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_coord_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<LifeCycleState StateType=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_life_cycle_type')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t\t\t<Indicators>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_life_cycle_indicators')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'</Indicators>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t\t</LifeCycleState>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<DangerZoneRadius';
    xml_str := xml_str || E' UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<LimitedDangerZoneRadius';
    xml_str := xml_str || E' UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area1_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area1')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<SafeRangeZoneRadius';
    xml_str := xml_str || E' UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_safe_area_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_safe_area')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<RadiationLevelNextToSource UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_rad_level_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_rad_level')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<DangerZoneRadiationLevel UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area_rad_level_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area_rad_level')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t\t<LimitedDangerZoneRadiationLevel UnitOfMeasurement=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area_rad_level1_uom')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E' Value=';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_dang_area_rad_level1')
    loop
       if (r.value is not null) then
            xml_str := xml_str || E'"' || r.value || E'"';
        end if;
    end loop;
    xml_str := xml_str || E'/>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t</Partial>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t</RadiationPolution>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t<EmergencySituationDocumentAppendix>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t\t<EvaciuationActions>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_evac_act')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</EvaciuationActions>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t\t<EmergencySituationLiquidationActions>';
    for r in
        select av.value from attrs_values av inner join io_objects io on (av.id_io_object=io.id and io.id=idObject) inner join attrs_categories ac on (av.id_attr_category = ac.id) inner join attributes a on (ac.id_io_attribute=a.id and a.code=E'shu_emerg_lic_act')
    loop
       if (r.value is not null) then
            xml_str := xml_str || r.value;
        end if;
    end loop;
    xml_str := xml_str || E'</EmergencySituationLiquidationActions>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t</EmergencySituationDocumentAppendix>\n';

    xml_str := xml_str || E'\t\t\t\t\t</RadiationPollutionDocument>\n';
    xml_str := xml_str || E'\t\t\t\t</EmergencySituation>\n';
    xml_str := xml_str || E'\t\t\t</Information>\n';
    xml_str := xml_str || E'\t\t</formalized_document_data>\n';
    xml_str := xml_str || E'\t\t<unformalized_document_data>\n';
    xml_str := xml_str || E'\t\t</unformalized_document_data>\n';
    xml_str := xml_str || E'\t\t<attached_files_set where_files_amnt="0">\n\t\t</attached_files_set>\n';
    xml_str := xml_str || E'\t</body>\n';
    xml_str := xml_str || E'\n</msg>';

    raise warning '%', xml_str;
    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);
    return result;
end
$BODY$
language 'plpgsql';
