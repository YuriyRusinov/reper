create or replace function aFindEqual(int4,
                                      varchar,
--                                      varchar,
                                      varchar,
                                      varchar,
                                      varchar,
                                      int4) returns int4 as
$BODY$
declare
    idAttrType alias for $1;
--    aCode alias for $2;
    aName alias for $2;
    aTitle alias for $3;
    aTable alias for $4;
    aColumn alias for $5;
    aDefWidth alias for $6;

    idAttr int4;
    r RECORD;
begin

    for r in 
        select id 
        from attributes
        where
            id_a_type = idAttrType
--            and code = aCode
            and name = aName
            and title = aTitle
            and (case when aTable is not null then table_name = aTable else table_name isnull end)
            and (case when aColumn is not null then column_name = aColumn else column_name isnull end)
            and def_width = aDefWidth
    loop
        return r.id;
    end loop;

    return -1;
end
$BODY$
language 'plpgsql';

create or replace function aFindEqualEx(int4,
                                      varchar,
                                      varchar,
                                      varchar,
                                      varchar,
                                      varchar,
                                      int4) returns int4 as
$BODY$
declare
    idAttrType alias for $1;
    aCode alias for $2;
    aName alias for $3;
    aTitle alias for $4;
    aTable alias for $5;
    aColumn alias for $6;
    aDefWidth alias for $7;

    idAttr int4;
    r RECORD;
begin

    for r in 
        select id 
        from attributes
        where
            id_a_type = idAttrType
            and code = aCode
            and name = aName
            and title = aTitle
            and (case when aTable is not null then table_name = aTable else table_name isnull end)
            and (case when aColumn is not null then column_name = aColumn else column_name isnull end)
            and def_width = aDefWidth
    loop
        return r.id;
    end loop;

    return -1;
end
$BODY$
language 'plpgsql';
