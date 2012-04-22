create or replace function eioInsertIndicator(int4, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar) returns int4 as
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
   

    idCategory int4;
    idCatAttr int4;
begin

    if(ioValue isnull) then
        return 1;
    end if;

    insert into rec_attrs_values(id_record, id_attr_category, value, start_time, stop_time, meas_time, id_io_object_src, id_io_object_src1, description)
    values (idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iMeasTime, iIdObjectSrc, iIdObjectSrc1, iDesc);
--    if(FOUND = FALSE) then
--        return -1;
--    end if;

    return 1;

end
$BODY$
language 'plpgsql';
