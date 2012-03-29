\encoding win1251

begin;
\i ./functions/init_data/readd_init_data.sql

/*
select f_safe_drop_trigger('trgcmdjinsert', 'command_journal');

\i ./functions/ik_tso/script_parsing-02.sql

create trigger trgCmdJInsert
before insert or update
on command_journal 
for each row 
execute procedure cmdJInsertCheck();
*/

commit;
