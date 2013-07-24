create or replace function eioUpdateIndicator(int8, int4, varchar, timestamp, timestamp, int4, int4, varchar) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idRecAttrValue int8;
begin


    idRecAttrValue = eioCatIndExist(idRec, idCategoryAttr);
    if(idRecAttrValue is not null and ioValue isnull) then
        delete from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr;
        return 0;
    end if;

    if(idRecAttrValue is not null) then
        update rec_attrs_values set 
            value = ioValue, 
            id_io_object_src = iIdObjectSrc, 
            id_io_object_src1 = iIdObjectSrc1, 
            description = iDesc 
        where 
            id = idRecAttrValue;
    else
        select eioInsertIndicator(idRec, idCategoryAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into idRecAttrValue;
        return idRecAttrValue;
    end if;

    return idRecAttrValue;
end
$BODY$
language 'plpgsql';

create or replace function eioCatIndExist(int8, int4) returns int8 as
$BODY$
declare
    idRec alias for $1;
    idCategoryAttr alias for $2;
    idRecAttrValue int8;
begin
    select id into idRecAttrValue from rec_attrs_values where id_record = idRec and id_attr_category = idCategoryAttr limit 1;
    if(idRecAttrValue isnull) then
        return NULL;
    end if;

    return idRecAttrValue;
end
$BODY$
language 'plpgsql' security definer;
