create or replace function fkQBaseTableCheck1() returns trigger as
$BODY$
declare
    theId int4;
begin
    if(TG_OP = 'UPDATE') then
        if(old.id <> new.id) then
            raise exception E'Change of primary keys on DynamicDocs\'s tables is unsupported!';
            return NULL;
        end if;
        return new;
    end if;

    delete from rec_attrs_values where id_record = old.id;
    delete from rubric_records where id_record = old.id;
    delete from rubric_records where id_rubric in (select id from recGetRubrics(old.id) where type in (0, 1));
    delete from record_rubricator where id in (select id from recGetRubrics(old.id) where type in (0, 1));
    delete from urls_records where id_record = old.id;

    return old;
end
$BODY$
language 'plpgsql';

--триггер создается каждый раз при создании очередного справочника (тиблицы справочника)

create or replace function createTriggerQBaseTableCheck1(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    --select f_safe_drop_trigger('trgsetuid', tableName);

    query = 'create trigger trg_fk_q_base_table_check1
             before update or delete
             on ' || tableName || '
             for each row 
             execute procedure fkQBaseTableCheck1();';

    execute query;

    return 1;

    exception when OTHERS then
        raise warning '%', 'CANNOT CREATE TRIGGER ON ' || tableName;
        return 0;
end
$BODY$
language 'plpgsql';
