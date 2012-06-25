create or replace function fkQBaseTableCheck() returns trigger as
$BODY$
declare
    theId int8;
    tName varchar;
begin
    if(TG_OP = 'UPDATE') then
        if(old.id_record = new.id_record) then
            return new;
        end if;
    end if;

    if (new.id_record is not null) then
        select id into theId from q_base_table where id = new.id_record;
        if(theId is null) then
            tName = TG_RELNAME;
            tName = E'insert or update on table "' || tName || E'" violates foreign key constraint to table q_base_table. \nDETAIL:  Key (id_record)=(' || new.id_record || E') is not present in table "q_base_table" or its childs.';
            raise exception '%', tName;
            return NULL;
        end if;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trg_fk_q_base_table_check', 'rec_attrs_values');
select f_create_trigger('trg_fk_q_base_table_check', 'before', 'insert or update', 'rec_attrs_values', 'fkqbasetablecheck()');

select f_safe_drop_trigger('trg_fk_q_base_table_check', 'record_rubricator');
select f_create_trigger('trg_fk_q_base_table_check', 'before', 'insert or update', 'record_rubricator', 'fkqbasetablecheck()');

select f_safe_drop_trigger('trg_fk_q_base_table_check', 'rubric_records');
select f_create_trigger('trg_fk_q_base_table_check', 'before', 'insert or update', 'rubric_records', 'fkqbasetablecheck()');

select f_safe_drop_trigger('trg_fk_q_base_table_check', 'urls_records');
select f_create_trigger('trg_fk_q_base_table_check', 'before', 'insert or update', 'urls_records', 'fkqbasetablecheck()');

