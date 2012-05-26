create or replace function rubricCheckAcc() returns trigger as
$BODY$
declare
    idUser int4;
begin

    select into idUser getCurrentUser ();
    insert into access_rubric_table (id_rubric, id_role, allow_read, allow_readlist, allow_delete, allow_update)
    values (new.id, idUser, true, true, true, true);

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgrubricinsertacc', 'rubricator');

create trigger trgrubricinsertacc
after insert
on rubricator 
for each row 
execute procedure rubricCheckAcc();
