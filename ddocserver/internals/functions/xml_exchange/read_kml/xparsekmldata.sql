drop table if exists kml_raw_data;
create table kml_raw_data(id serial, kml_doc varchar, constraint pk_id primary key (id));


drop table if exists kml_placemark_styles cascade;
create table kml_placemark_styles (
   id                   serial               not null,
   style_uid            varchar              not null,
   style_string         varchar              null,
   rsc_uid              varchar              not null,
   rsc_name             varchar              not null,
   constraint PK_KML_PLACEMARK_STYLES primary key (id)
);

insert into kml_placemark_styles (style_uid, rsc_uid, rsc_name) values ('default_point', '1', 'topo200');
insert into kml_placemark_styles (style_uid, rsc_uid, rsc_name) values ('default_linestring', '2', 'topo200');
insert into kml_placemark_styles (style_uid, rsc_uid, rsc_name) values ('default_polygon', '3', 'topo200');

insert into kml_placemark_styles (style_uid, rsc_uid, rsc_name) values ('style_520', '111', 'topo200');
insert into kml_placemark_styles (style_uid, rsc_uid, rsc_name) values ('style_521', '112', 'topo200');

drop table if exists kml_placemarks_lines;
create table kml_placemarks_lines (
   id                   serial               not null,
   placemark_style_id   INT4                 not null,
   name                 varchar              not null,
   placemark_uid        varchar              not null,
   placemark_style_string varchar              null,
   placemark_geom       geometry             not null,
   placemark_name       varchar              not null,
   constraint PK_KML_PLACEMARKS_LINES primary key (id)
);

alter table kml_placemarks_lines
   add constraint FK_KML_PLAC_REFERENCE_KML_PLAC foreign key (placemark_style_id)
      references kml_placemark_styles (id)
      on delete restrict on update restrict;


drop table if exists kml_placemarks_points;
create table kml_placemarks_points (
   id                   serial               not null,
   placemark_style_id   INT4                 not null,
   name                 varchar              not null,
   placemark_uid        varchar              not null,
   placemark_style_string varchar              null,
   placemark_geom       geometry             not null,
   placemark_name       varchar              not null,
   constraint PK_KML_PLACEMARKS_POINTS primary key (id)
);

alter table kml_placemarks_points
   add constraint FK_KML_PLAC_REFERENCE_KML_PLAC foreign key (placemark_style_id)
      references kml_placemark_styles (id)
      on delete restrict on update restrict;

drop table if exists kml_placemarks_poly;
create table kml_placemarks_poly (
   id                   serial               not null,
   placemark_style_id   INT4                 not null,
   name                 varchar              not null,
   placemark_uid        varchar              not null,
   placemark_style_string varchar              null,
   placemark_geom       geometry             not null,
   placemark_name       varchar              not null,
   constraint PK_KML_PLACEMARKS_POLY primary key (id)
);

alter table kml_placemarks_poly
   add constraint FK_KML_PLAC_REFERENCE_KML_PLAC foreign key (placemark_style_id)
      references kml_placemark_styles (id)
      on delete restrict on update restrict;




create or replace function trg_parse_kml() returns trigger as
$BODY$
declare
begin

    perform kmlParseDocument(new.kml_doc::xml);

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgkmlrawdatainsert', 'kml_raw_data');

select f_create_trigger('trgkmlrawdatainsert', 'before', 'insert', 'kml_raw_data', 'trg_parse_kml()');


create or replace function kmlParseDocument(xml) returns int4 as
$BODY$
declare
    kmlBody alias for $1;

    docBody xml;
    r record;
    res int4;
    pos int4;
    nsUri varchar;
    nsgxUri varchar;
    q varchar;
    arrs varchar;
begin

    nsUri = '';
    select strpos(kmlBody::varchar, ' xmlns="') into pos;
    if(pos > 0) then
        select split_part(kmlBody::varchar, ' xmlns="', 2) into nsUri;
        select strpos( nsUri, '"') into pos;
        select substr(nsUri, 1, pos-1) into nsUri;
        SELECT btrim(array_to_string(xpath('/my:kml/my:Document', kmlBody, ARRAY[ARRAY['my', nsUri]]), '', ''))::varchar into docBody;
    else
        SELECT btrim(array_to_string(xpath('/kml/Document', kmlBody), '', ''))::xml into docBody;
    end if;

    nsgxUri = '';
    select strpos(kmlBody::varchar, ' xmlns:gx="') into pos;
    if(pos > 0) then
        select split_part(kmlBody::varchar, ' xmlns:gx="', 2) into nsgxUri;
        select strpos( nsgxUri, '"') into pos;
        select substr(nsgxUri, 1, pos-1) into nsgxUri;
    end if;

    arrs = '';
    if(nsUri <> '') then
        q = 'select unnest(xpath(' || quote_literal('/my:kml/my:Document/my:Placemark') || ', ' || quote_literal(kmlBody);
        arrs = 'ARRAY[ARRAY[' || quote_literal('my') || ', ' || quote_literal(nsUri) || ']';
    else
        q = 'select unnest(xpath(' || quote_literal('/kml/Document/Placemark') || ', ' || quote_literal(kmlBody);
    end if;

    if(nsgxUri <> '') then
        if(arrs = '') then
            arrs = 'ARRAY[';
        else
            arrs = arrs || ', ';
        end if;
        
        arrs = arrs || 'ARRAY[' || quote_literal('gx') || ', ' || quote_literal(nsgxUri) || ']';
    end if;

    if(arrs <> '') then
        arrs = arrs || ']';
    end if;

    if(arrs <> '') then
        q = q || ', ' || arrs || ')) as kml_placemark';
    else
        q = q || ')) as kml_placemark';
    end if;
    --raise exception '%', q;
    --for r in select unnest(xpath('/Document/Placemark', docBody)) as kml_placemark
    for r in execute q
    loop
        select kmlParsePlacemark(r.kml_placemark, kmlBody, arrs) into res;
        if(res <= 0) then
            return -1;
        end if;
    end loop;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function kmlParsePlacemark(xml, xml, varchar) returns int4 as
$BODY$
declare
    kmlBody alias for $1;
    fullDoc alias for $2; -- for styles parsing purposes
    arrs alias for $3; --namespace aliases

    placemarkUid varchar;
    placemarkStyleId varchar;
    placemarkStyleString varchar;
    placemarkType int4;
    geoPart varchar;
    xpathForStyle varchar;

    placemarkName varchar;

    res int4;
    placemarkId int8; --internal id in ddocserver database
    q varchar;
begin

    --identify geodata type
    placemarkType = 0; -- undefined

    SELECT btrim(array_to_string(xpath('/Placemark/Point', kmlBody), '', ''))::varchar into geoPart;
    if(geoPart is not null and  geoPart <> '') then
        placemarkType = 1; --point
    else
        SELECT btrim(array_to_string(xpath('/Placemark/LineString', kmlBody), '', ''))::varchar into geoPart;
        if(geoPart is not null and  geoPart <> '') then
            placemarkType = 2; --linestring
        else
            SELECT btrim(array_to_string(xpath('/Placemark/Polygon', kmlBody), '', ''))::varchar into geoPart;
            if(geoPart is not null and  geoPart <> '') then
               placemarkType = 3; --polygon
            else
                return -1; --incorrect placemark;
            end if;

        end if;

    end if;

    if(placemarkType = 0) then
        return -1; --undefined placemark type
    end if;

    --try to detect uid of placemark
    SELECT btrim(array_to_string(xpath('/Placemark/@id', kmlBody), '', ''))::varchar into placemarkUid;
    if(placemarkUid isnull or placemarkUid = '') then
        SELECT btrim(array_to_string(xpath('/Placemark/id/text()', kmlBody), '', ''))::varchar into placemarkUid;
    end if;

    if(placemarkUid isnull or placemarkUid = '') then
        placemarkUid = generateUid()::varchar;
    end if;

    --try to detect placemark style
    SELECT btrim(array_to_string(xpath('/Placemark/styleUrl/text()', kmlBody), '', ''))::varchar into placemarkStyleId;
    if(placemarkStyleId isnull or placemarkStyleId = '') then
        placemarkStyleId = 'defaultStyle';
    else
        if(substring(placemarkStyleId from 1 for 1) = '#') then
            placemarkStyleId = overlay(placemarkStyleId placing '' from 1 for 1);
        end if;

        if(arrs <> '' and strpos(arrs, 'ARRAY[' || quote_literal('my')) > 0 ) then
            xpathForStyle = '/my:kml/my:Document/my:Style[@id="';
        else
            xpathForStyle = '/kml/Document/Style[@id="';
        end if;
        
        if(arrs <> '') then
            q = 'SELECT btrim(array_to_string(xpath(' || quote_literal(xpathForStyle || asString(placemarkStyleId, false) || '"]') || ', ' || quote_literal(fullDoc) || ', ' || arrs || '), ' || quote_literal('') || ', ' || quote_literal('') || '))::varchar';
        else
            q = 'SELECT btrim(array_to_string(xpath(' || quote_literal(xpathForStyle || asString(placemarkStyleId, false) || '"]') || ', ' || quote_literal(fullDoc) || '), ' || quote_literal('') || ', ' || quote_literal('') || '))::varchar';
        end if;
        --raise exception '%', q;
        execute q into placemarkStyleString;
        if(placemarkStyleString isnull or placemarkStyleString = '') then
            placemarkStyleString = '';
        end if;
    end if;


    --get all attributes (now name only)
    SELECT btrim(array_to_string(xpath('/Placemark/name/text()', kmlBody), '', ''))::varchar into placemarkName;
    if(placemarkName isnull or placemarkName = '') then
        placemarkName = '';
    end if;
    
    placemarkId = kmlGetEqualPlacemark(placemarkUid, placemarkType);
    if(placemarkId is not null and placemarkId > 0) then
        res = kmlUpdatePlacemark(placemarkId, placemarkType, placemarkName, placemarkStyleId, placemarkStyleString, geoPart);
        if(res isnull or res <= 0) then
            raise exception 'Cannot update placemark in database! Id = %, res = %', placemarkId, res;
            return -1;
        end if;
    else
        placemarkId = kmlInsertPlacemark(placemarkUid, placemarkType, placemarkName, placemarkStyleId, placemarkStyleString, geoPart);
        if(placemarkId isnull or placemarkId <= 0) then
            raise exception 'Cannot insert new placemark into database! uid = %, res = %', placemarkUid, placemarkId;
            return -2;
        end if;
    end if;

    return placemarkId;
end
$BODY$
language 'plpgsql';

create or replace function kmlGetStyleInternalId(varchar, int4) returns int8 as
$BODY$
declare
    placemarkStyleId alias for $1;
    placemarkType alias for $2;

    theId int8;
begin

    theId = 0;

    if(placemarkStyleId is not null) then
        select id into theId from kml_placemark_styles where style_uid = placemarkStyleId;
    end if;
    
    if(theId isnull or theId <= 0) then

        if(placemarkType = 1) then --points
            return 1;
        elsif(placemarkType = 2) then --linestrings
            return 2;
        else
            return 3;
        end if;

    end if;

    return theId;

end
$BODY$
language 'plpgsql';


create or replace function kmlInsertPlacemark(varchar, int4, varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    placemarkUid alias for $1;
    placemarkType alias for $2;
    placemarkName alias for $3;
    placemarkStyleId alias for $4;
    placemarkStyleString alias for $5;
    geoPart alias for $6;

    placemarkId int8;
    placemarkStyleInternalId int8;
    tableName varchar;
    --placemarkGeomString varchar;
    geom geometry;
    q varchar;
    rCount int4;
begin


    placemarkStyleInternalId = kmlGetStyleInternalId(placemarkStyleId, placemarkType);

    --raise exception '%', geoPart;
    geom = ST_GeomFromKML(geoPart);
    
    tableName = kmlGetPlacemarkTable(placemarkType);

    placemarkId = getNextSeq(tableName, 'id');

    q = 'insert into ' || tableName || ' (id, name, placemark_uid, placemark_style_id, placemark_style_string, placemark_geom, placemark_name) values (' ||
                                         placemarkId || ', ' ||
                                         asString(placemarkName, true) || ', ' ||
                                         asString(placemarkUid, true) || ', ' ||
                                         placemarkStyleInternalId || ', ' ||
                                         asString(placemarkStyleString, true) || ', ' ||
                                         asString(geom, true) || ', ' ||
                                         asString(placemarkName, true) || ')';

    execute q;

    get diagnostics rCount = ROW_COUNT;

    if(rCount <= 0) then
        return -1;
    end if;

    return placemarkId;

end
$BODY$
language 'plpgsql';

create or replace function kmlUpdatePlacemark(int8, int4, varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    placemarkId alias for $1;
    placemarkType alias for $2;
    placemarkName alias for $3;
    placemarkStyleId alias for $4;
    placemarkStyleString alias for $5;
    geoPart alias for $6;
 
    placemarkStyleInternalId int8;
    tableName varchar;
    --placemarkGeomString varchar;
    geom geometry;
    q varchar;
    rCount int4;
begin

    placemarkStyleInternalId = kmlGetStyleInternalId(placemarkStyleId, placemarkType);

    geom = ST_GeomFromKML(geoPart);

    tableName = kmlGetPlacemarkTable(placemarkType);
    
    q = 'update ' || tableName || ' set ' || 
                                       'name = ' || asString(placemarkName, true) || ', ' ||
                                       --'placemark_uid = ' || || ', ' ||
                                       'placemark_style_id = ' || placemarkStyleInternalId || ', ' ||
                                       'placemark_style_string = ' || asString(placemarkStyleString, true) || ', ' ||
                                       'placemark_geom = ' || asString(geom, true) || ', ' ||
                                       'placemark_name = ' || asString(placemarkName, true) || 
                                   ' where id = ' || placemarkId;

    execute q;

    get diagnostics rCount = ROW_COUNT;

    if(rCount <= 0) then
        return -1;
    end if;

    return placemarkId;

end
$BODY$
language 'plpgsql';


create or replace function kmlGetPlacemarkTable(int4) returns varchar as
$BODY$
declare
    placemarkType alias for $1;
begin

    if(placemarkType = 1) then --point
        return 'kml_placemarks_points';
    elsif(placemarkType = 2) then --linestring
        return 'kml_placemarks_lines';
    elsif(placemarkType = 3) then --polygon
        return 'kml_placemarks_poly';
    else
        return '';
    end if;

    return NULL;

end 
$BODY$
language 'plpgsql';

create or replace function kmlGetEqualPlacemark(varchar, int4) returns int8 as
$BODY$
declare
    placemarkUid alias for $1;
    placemarkType alias for $2;

    tableName varchar;
    columnName varchar;

    placemarkId int8;
    r record;
    q varchar;
begin

    tableName = kmlGetPlacemarkTable(placemarkType);
    if(tableName isnull or tableName = '') then
        return NULL;
    end if;

    columnName = 'placemark_uid';

    q = 'select id from ' || tableName || ' where "' || columnName || '" = ' || quote_literal(placemarkUid);
    for r in execute q
    loop
        placemarkId = r.id;
    end loop;

    return placemarkId;

end
$BODY$
language 'plpgsql';

create or replace function kmlParseLineString(xml) returns varchar as
$BODY$
declare
    kmlBody alias for $1;

    geoPart varchar;
begin

    SELECT btrim(array_to_string(xpath('/Placemark/LineString/text()', xmlBody), '', ''))::varchar into geoPart;
    if(geoPart isnull or geoPart = '') then
        return NULL;
    end if;

    return geoPart;
end
$BODY$
language 'plpgsql';


create or replace function kmlParsePoint(xml) returns varchar as
$BODY$
declare
    kmlBody alias for $1;

    geoPart varchar;
begin

    SELECT btrim(array_to_string(xpath('/Placemark/Point/text()', xmlBody), '', ''))::varchar into geoPart;
    if(geoPart isnull or geoPart = '') then
        return NULL;
    end if;

    return geoPart;
end
$BODY$
language 'plpgsql';

create or replace function kmlParsePolygon(xml) returns varchar as
$BODY$
declare
    kmlBody alias for $1;

    geoPart varchar;
begin

    SELECT btrim(array_to_string(xpath('/Placemark/Polygon/text()', xmlBody), '', ''))::varchar into geoPart;
    if(geoPart isnull or geoPart = '') then
        return NULL;
    end if;

    return geoPart;
end
$BODY$
language 'plpgsql';


