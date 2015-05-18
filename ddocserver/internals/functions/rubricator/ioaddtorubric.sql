create or replace function ioAddToRubric (int4, int4, VARCHAR) returns int4 as
$BODY$
declare
    idObject alias for $1;
    idRubric alias for $2;
    rIcon alias for $3;

    query varchar;
    cnt int4;
    r record;
    rubr_query varchar;
    rr record;
begin

    select count(*) into cnt from io_rubricator where id_io_object = idObject and id_rubric = idRubric;
    if (cnt is not null and cnt > 0) then
        update io_rubricator set r_icon  = rIcon where id_io_object = idObject and id_rubric = idRubric;
        return 1;
    end if;
    
    insert into io_rubricator (id_rubric, id_io_object, is_automated, r_icon) values (idRubric, idObject, false, rIcon);

    return 1;
end
$BODY$
language 'plpgsql';
