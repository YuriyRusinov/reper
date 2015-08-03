create or replace function ioInsertCheckAcc() returns trigger as
$BODY$
declare
    id_user int4;
begin

    id_user := getCurrentUser ();
    if ( id_user = 2) then  --jupiter
        insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
        values(new.id, -10, true, true, false, false, false);
    else
        insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
        values(new.id, new.author, true, true, true, true, true);
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgioinsertacc', 'io_objects');

select f_create_trigger('trgioinsertacc', 'after', 'insert', 'io_objects', 'ioinsertcheckacc()');

/*
create trigger trgIOInsertAcc 
after insert
on tbl_io_objects 
for each row 
execute procedure ioInsertCheckAcc();
*/
