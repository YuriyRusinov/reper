create or replace function eioUpdateIndicator(int8, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar) returns int4 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iMeasTime alias for $6;
    iIdObjectSrc alias for $7;
    iIdObjectSrc1 alias for $8;
    iDesc alias for $9;

    ok int4;
begin


    if(ioValue isnull) then
        delete from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr;
        return 1;
    end if;

    if(eioCatIndExist(idRec, idCategoryAttr) = true) then
        update rec_attrs_values set 
            value = ioValue, 
            meas_time = iMeasTime, 
            id_io_object_src = iIdObjectSrc, 
            id_io_object_src1 = iIdObjectSrc1, 
            description = iDesc 
        where 
            id_record = idRec
            and id_attr_category = idCategoryAttr;
    else
        select eioInsertIndicator(idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iMeasTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
        return ok;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function eioCatIndExist(int8, int4) returns bool as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    cnt int4;
begin
    select count(*) into cnt from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr;
    if(cnt > 0) then
        return true;
    end if;

    return false;
end
$BODY$
language 'plpgsql' security definer;
