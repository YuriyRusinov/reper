create or replace function rubricCheckAcc() returns trigger as
$BODY$
declare
    idUser int4;
begin

    if(TG_OP = 'INSERT') then
        select into idUser getCurrentUser ();
        insert into access_rubric_table (id_rubric, id_role, allow_read, allow_readlist, allow_delete, allow_update)
        values (new.id, idUser, true, true, true, true);
    end if;

    if(new.id_search_template is not null) then
        perform putIOIntoRubric(new.id);
    end if;

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trgrubricinsertacc', 'rubricator');

create trigger trgrubricinsertacc
after insert or update
on rubricator 
for each row 
execute procedure rubricCheckAcc();
