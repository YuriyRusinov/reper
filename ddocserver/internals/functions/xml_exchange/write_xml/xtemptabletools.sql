create or replace function xCreateTempTable() returns int4 as
$BODY$
begin

    create temp table x_xml_temp_table 
    (    

       id                   SERIAL not null,
       type                 int2 not null, -- 1 - IO, 2 - record, 3 - category
       id_object            int4, --id_object in which the record contains (only for records!)
       id_entity            int8   not null, --id_record or id_category
       uid                  varchar not null, --uuid for IO, records, id - for categories
       xml_text             varchar not null,
       constraint PK_x_xml_key primary key (id)
    );

    create unique index i_x_xml_temp_table on x_xml_temp_table (
        type,
        id_object,
        id_entity
    );

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xDropTempTable() returns int4 as
$BODY$
begin

    drop table x_xml_temp_table;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function xInsertIntoTempTable(int2, int4, int8, varchar, varchar) returns int4 as
$BODY$
declare
    iType alias for $1;
    idObject alias for $2;
    idEntity alias for $3;
    iUid alias for $4;
    iXml alias for $5;

    iId int4;
begin

    if(iType isnull or iType < 1 or iType > 3) then
        raise exception 'Invalid type of entity! 1, 2 or 3 is only allowed!';
        return -1;
    end if;

    if(iType <> 2) then
        select id into iId from x_xml_temp_table where type = iType and id_entity = idEntity;
    else --records
        select id into iId from x_xml_temp_table where type = iType and id_object = idObject and id_entity = idEntity;
    end if;
    
    if(iId is not null) then
        return iId;
    end if;

    select pg_catalog.nextval('x_xml_temp_table_id_seq') into iId;
    if(iId isnull) then
        raise exception 'Cannot get sequense value for x_xml_temp_table!';
        return -1;
    end if;

    insert into x_xml_temp_table (id, type, id_object, id_entity, uid, xml_text) values (iId, iType, idObject, idEntity, iUid, iXml);

    return iId;

end
$BODY$
language 'plpgsql';


create or replace function xGetPreparedCategories() returns varchar as
$BODY$
declare
    res varchar;
    r record;
begin

    res = '';
    
    for r in select xml_text from x_xml_temp_table where type = 3 order by id
    loop
        res = res || r.xml_text;
    end loop;
    --select string_agg(xml_text, ' ') into res from x_xml_temp_table where type = 3 order by id;
    return res;

end
$BODY$
language 'plpgsql';


create or replace function xGetPreparedRecords() returns varchar as
$BODY$
declare
    res varchar;
    r record;
begin

    res = '';
    
    for r in select xml_text from x_xml_temp_table where type = 2 order by id
    loop
        res = res || r.xml_text;
    end loop;
    --select string_agg(xml_text, ' ') into res from x_xml_temp_table where type = 2 order by id;
    return res;

end
$BODY$
language 'plpgsql';

create or replace function xIsCategoryPrepared(int4) returns varchar as
$BODY$
declare
    idCategory alias for $1;
    iUid varchar;
begin

    select uid into iUid from x_xml_temp_table where type = 3 and id_entity = idCategory and id_object = NULL;
    if(iUid is not null) then
        return iUid;
    end if;
 
    return '';

end
$BODY$
language 'plpgsql';

create or replace function xIsRecordPrepared(int4, int8) returns varchar as
$BODY$
declare
    idObject alias for $1;
    idRecord alias for $2;
    iUid varchar;
begin

    select uid into iUid from x_xml_temp_table where type = 2 and id_entity = idRecord and id_object = idObject;
    if(iUid is not null) then
        return iUid;
    end if;
 
    return '';

end
$BODY$
language 'plpgsql';
