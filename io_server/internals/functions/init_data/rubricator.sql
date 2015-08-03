select f_safe_drop_trigger('trgrubricinsertacc', 'rubricator');


insert into rubricator (unique_id, id_parent, id_io_object, id_search_template, id_io_category, name, code, description)
values ('localorg-rubricator-1', NULL, NULL, NULL, NULL, 'Общесистемный рубрикатор', NULL, NULL);
insert into access_rubric_table (id_rubric, id_role, allow_read, allow_readlist, allow_delete, allow_update)
values (1, -10, true, true, true, true);

select f_create_trigger('trgrubricinsertacc', 'after', 'insert', 'rubricator', 'rubriccheckacc()');

/*
create trigger trgrubricinsertacc
after insert
on rubricator 
for each row 
execute procedure rubricCheckAcc();
*/
