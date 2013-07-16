-- Function: sd_ioupdateattr(integer, integer, character varying, timestamp without time zone, timestamp without time zone, integer, integer, character varying)

-- DROP FUNCTION sd_ioupdateattr(integer, integer, character varying, timestamp without time zone, timestamp without time zone, integer, integer, character varying);

CREATE OR REPLACE FUNCTION sd_ioupdateattr(integer, integer, character varying, timestamp without time zone, timestamp without time zone, integer, integer, character varying)
  RETURNS integer AS
$BODY$
declare
    idObject alias for $1;
    idAttr alias for $2;
    ioValue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    idCategory int4;
    idCatAttr int4;
    ok int4;
    query varchar;
begin

    idCatAttr = acGetAttrCategoryIdByIO(idObject, idAttr);

    if(ioValue isnull) then
        delete from attrs_values where id_io_object = idObject and id_attr_category = idCatAttr;
        return 1;
    end if;

    if(ioCatAttrExist(idObject, idCatAttr) ) then

	update attrs_values set
				value = ioValue,
				stop_time=iStopTime,
				start_time = iStartTime,
				id_io_object_src=iIdObjectSrc,
				id_io_object_src1=iIdObjectSrc1,
				description=iDesc
	where id_io_object=idObject and id_attr_category=idCatAttr;
	
    else
        select ioInsertAttr(idObject, idAttr, ioValue, iStartTime, iStopTime, iIdObjectSrc, iIdObjectSrc1, iDesc) into ok;
        return ok;
    end if;

    return 1;
end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION sd_ioupdateattr(integer, integer, character varying, timestamp without time zone, timestamp without time zone, integer, integer, character varying)
  OWNER TO admin;
