create or replace function eioInsertIndicator(int8, int4, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
--    iMeasTime alias for $6;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;
   

    idCategory int4;
    idCatAttr int4;
    idRecAttrValue int8;
begin

    if(ioValue isnull) then
        return 0;
    end if;

    select getNextSeq('rec_attrs_values', 'id') into idRecAttrValue;

    insert into rec_attrs_values(id, id_record, id_attr_category, value, start_time, stop_time, id_io_object_src, id_io_object_src1, description)
    values (idRecAttrValue, idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc);
--    if(FOUND = FALSE) then
--        return -1;
--    end if;

    return idRecAttrValue;

end
$BODY$
language 'plpgsql';
