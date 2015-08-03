-- Function: sd_ioseticon(integer, character varying)

-- DROP FUNCTION sd_ioseticon(integer, character varying);

CREATE OR REPLACE FUNCTION sd_ioseticon(integer, character varying)
  RETURNS integer AS
$BODY$
declare

    idObject alias for $1;
    rIcon alias for $2;

begin

    if (idObject is null or idObject <= 0) then
        return -1;
    end if;
    
    update io_objects set 
                            r_icon = rIcon
    where id=idObject;


    return idObject;

end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION sd_ioseticon(integer, character varying)
  OWNER TO admin;