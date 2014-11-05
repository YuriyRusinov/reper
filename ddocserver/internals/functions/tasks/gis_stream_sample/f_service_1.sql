create or replace function f_service_1(int4) returns int4 as
$BODY$
declare
    idChainsData alias for $1;
    r record;
    rr record;
    idCategory int4;

    objId int8;
    objNum varchar;
    objType int8;
    objCoords varchar;
    objComment varchar;
    objMeasTime timestamp;

    res int4;
begin

    res = 0;
    
    for r in 
        select * from chains_data where id = idChainsData
    loop
        select id_io_category into idCategory from io_objects where id = r.id_io_object;
        if(idCategory <> 301) then
            return 0;
        end if;

        for rr in 
            select * from ioGetObjectAttrs(r.id_io_object)
        loop
            if(rr.id_io_attribute = 1) then
                objId = to_int8(rr.value);
            elsif(rr.id_io_attribute = 1001) then
                objNum = rr.value;
            elsif(rr.id_io_attribute = 1002) then
                objType = to_int8(rr.value);
            elsif(rr.id_io_attribute = 1003) then
                objCoords = rr.value;
            elsif(rr.id_io_attribute = 1004) then
                objComment = rr.value;
            elsif(rr.id_io_attribute = 1005) then
                objMeasTime = to_timestamp(rr.value);
            else
                return -1;
            end if;
        end loop;

        if(objType = 1) then --points
            res = f_service_2_helper(objNum, objType, objCoords, objComment, objMeasTime);
        elsif(objType = 2) then --lines
            res = f_service_3_helper(objNum, objType, objCoords, objComment, objMeasTime);
        elsif(objType = 3) then --polygons
            res = f_service_4_helper(objNum, objType, objCoords, objComment, objMeasTime);
        else
            return -2;
        end if;
        
    end loop;
    
    return res;
end
$BODY$
language 'plpgsql';


create or replace function f_service_2_helper(varchar, int8, varchar, varchar, timestamp) returns int4 as
$BODY$
declare

    objNum alias for $1;
    objType alias for $2;
    objCoords alias for $3;
    objComment alias for $4;
    objMeasTime alias for $5;

    objCoordsAsGeometry geometry;
    objVelosity float8;
    existingId int8;

    r record;
    oldMeasTime timestamp;
    oldCoords geometry;
    objDistance float8;
    cntSeconds float8;

begin

    if(objType <> 1) then
        return -1;
    end if;

    objCoordsAsGeometry = st_geometryFromText(objCoords, 4326); --wgs84

    existingId = NULL;
    for r in 
        select 
            id, 
            "o2-attributes-1006" as measTime, 
            "o2-attributes-1008" as oldGeom 
        from eio_table_303 where "o2-attributes-1001" = objNum
    loop
        existingId = r.id;
        oldMeasTime = r.measTime;
        oldCoords = r.oldGeom;
    end loop;

    if(existingId isnull) then
        objVelosity = 0.0;
        insert into eio_table_303 ("o2-attributes-1001", 
                                   "o2-attributes-1002", 
                                   "o2-attributes-1006", 
                                   "o2-attributes-1007", 
                                   "o2-attributes-1008", 
                                   "o2-attributes-1011") 
                            values(objNum, 
                                   objType,
                                   objMeasTime,
                                   objComment,
                                   objCoordsAsGeometry,
                                   objVelosity
                                   );
    else

        if(oldMeasTime isnull) then
            oldMeasTime = objMeasTime;
        end if;
        if(oldCoords isnull) then
            oldCoords = objCoordsAsGeometry;
        end if;

        objDistance = ST_Distance_Sphere(objCoordsAsGeometry, oldCoords);
        if(oldMeasTime = objMeasTime or objDistance = 0.0) then
            objVelosity = 0.0;
        else
            cntSeconds = getSeconds(objMeasTime - oldMeasTime);
            if(cntSeconds isnull or cntSeconds = 0.0) then
                objVelosity = 0.0;
            else
                objVelosity = objDistance / cntSeconds;
            end if;
        end if;
        
        update eio_table_303 set 
                                 "o2-attributes-1006" = objMeasTime, 
                                 "o2-attributes-1007" = objComment, 
                                 "o2-attributes-1008" = objCoordsAsGeometry,
                                 "o2-attributes-1011" = objVelosity
        where "o2-attributes-1001" = objNum;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';


create or replace function f_service_3_helper(varchar, int8, varchar, varchar, timestamp) returns int4 as
$BODY$
declare

    objNum alias for $1;
    objType alias for $2;
    objCoords alias for $3;
    objComment alias for $4;
    objMeasTime alias for $5;

    objCoordsAsGeometry geometry;
    objLength float8;
    existingId int8;

    r record;

begin

    if(objType <> 2) then
        return -1;
    end if;

    objCoordsAsGeometry = st_geometryFromText(objCoords, 4326); --wgs84
    objLength = ST_Length_Spheroid(objCoordsAsGeometry, 'SPHEROID["WGS 84",6378137,298.257223563]');

    existingId = NULL;
    for r in 
        select 
            id
        from eio_table_301 where "o2-attributes-1001" = objNum
    loop
        existingId = r.id;
    end loop;

    if(existingId isnull) then
        
        insert into eio_table_301 ("o2-attributes-1001", 
                                   "o2-attributes-1002", 
                                   "o2-attributes-1006", 
                                   "o2-attributes-1007", 
                                   "o2-attributes-1008", 
                                   "o2-attributes-1009") 
                            values(objNum, 
                                   objType,
                                   objMeasTime,
                                   objComment,
                                   objCoordsAsGeometry,
                                   objLength
                                   );
    else
        
        update eio_table_301 set 
                                 "o2-attributes-1006" = objMeasTime, 
                                 "o2-attributes-1007" = objComment, 
                                 "o2-attributes-1008" = objCoordsAsGeometry,
                                 "o2-attributes-1009" = objLength
        where "o2-attributes-1001" = objNum;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

create or replace function f_service_4_helper(varchar, int8, varchar, varchar, timestamp) returns int4 as
$BODY$
declare

    objNum alias for $1;
    objType alias for $2;
    objCoords alias for $3;
    objComment alias for $4;
    objMeasTime alias for $5;

    objCoordsAsGeometry geometry;
    objGeography geography;
    
    objArea float8;
    existingId int8;

    r record;

begin

    if(objType <> 3) then
        return -1;
    end if;

    objCoordsAsGeometry = st_geometryFromText(objCoords, 4326); --wgs84
    objGeography = st_geographyFromText('SRID=4326;' || objCoords);
    objArea = st_area(objGeography, true);

    existingId = NULL;
    for r in 
        select 
            id
        from eio_table_302 where "o2-attributes-1001" = objNum
    loop
        existingId = r.id;
    end loop;

    if(existingId isnull) then
        
        insert into eio_table_302 ("o2-attributes-1001", 
                                   "o2-attributes-1002", 
                                   "o2-attributes-1006", 
                                   "o2-attributes-1007", 
                                   "o2-attributes-1008", 
                                   "o2-attributes-1010") 
                            values(objNum, 
                                   objType,
                                   objMeasTime,
                                   objComment,
                                   objCoordsAsGeometry,
                                   objArea
                                   );
    else
                
        update eio_table_302 set 
                                 "o2-attributes-1006" = objMeasTime, 
                                 "o2-attributes-1007" = objComment, 
                                 "o2-attributes-1008" = objCoordsAsGeometry,
                                 "o2-attributes-1010" = objArea
        where "o2-attributes-1001" = objNum;
    end if;

    return 1;

end
$BODY$
language 'plpgsql';

/*

select * from chains_data
select * from io_categories where id > 300 order by id

select * from eio_table_301

select * from io_objects where id > 300 order by id

select * from cGetCategoryAttrs(306)


select getSeconds('01.01.2014 00:10:00'::timestamp - '01.01.2014 00:05:00'::timestamp)

select * from spatial_ref_sys

select * from st_geometryFromtext('POINT(0 0)', 4326); --wgs84
*/