create or replace function checkTableForOwner() returns trigger as
$BODY$
declare
    theTable varchar;
    idObject int4;
begin
    
    theTable = TG_RELNAME;
    if(substr(theTable, 1, 4) = 'tbl_') then
        theTable = substr(theTable, 5);
    end if;

    --hack for id_io_state
    if(TG_OP = 'INSERT' or TG_OP = 'UPDATE') then
        if(new.id_io_state is null) then
            new.id_io_state = 1; --active
        end if;
    end if;

    select id into idObject from f_sel_io_objects_tbl(theTable) where table_name = theTable;
    if(idObject isnull) then
        select id_io_object into idObject from object_ref_tables where table_name = theTable;
        if(idObject isnull) then
            raise exception 'WRONG TRIGGER ASSOCIATED WITH NONOBJECT TABLE!';
            return NULL;
        end if;
    end if;

    if(checkIOForOwner(idObject) = false) then
        return NULL;
    end if;

    if(TG_OP = 'DELETE') then
        return old;
    end if;
    
    return new;
end
$BODY$
language 'plpgsql';
