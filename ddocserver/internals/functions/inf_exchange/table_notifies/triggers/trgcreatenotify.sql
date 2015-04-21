/* asyncronous notification for qualifiers */

create or replace function trgCreateNotifyTrigger() returns trigger as
$BODY$
declare
    tableName varchar;

    --params and criteria for notify generation
    notifyName varchar; 
    idPosition int4; 
    idUnit int4;
    idSearchTemplate int4;
    isAccept bool;

    triggerName varchar;
    qualifierTableName varchar;
    notifyWhere varchar;
    trgFunc varchar;
    nWheres int4[];
    nName varchar;
    cnt int4;
    i int4;
    theIdTableNotifies int4;
    theIdObject int4;
    r record;
begin

    tableName = TG_RELNAME;
    if(tableName <> 'table_notifies_io_objects') then
        return NULL;
    end if;
  
    if(TG_OP = 'UPDATE') then
        return NULL;
    end if;

    if(TG_OP = 'INSERT') then
        theIdTableNotifies = new.id_table_notifies;
        theIdObject = new.id_io_objects;
    else
        theIdTableNotifies = old.id_table_notifies;
        theIdObject = old.id_io_objects;
    end if;

    for r in 
        select name, id_position, id_unit, id_search_template, is_accept, notify_where
        from table_notifies 
        where id = theIdTableNotifies
    loop
        notifyName = lower(r.name);
        idPosition = r.id_position;
        idUnit = r.id_unit;
        isAccept = r.is_accept;
        idSearchTemplate = r.id_search_template;
        nWheres = r.notify_where;
    end loop;

    if(notifyName isnull or nWheres isnull) then
        return NULL;
    end if;

    triggerName = 'trgasyncnotify_' || notifyName;

    select table_name into qualifierTableName from tbl_io_objects where id = theIdObject;
    if(qualifierTableName isnull) then
        return NULL;
    end if;

    if(theIdObject > 300) then
        qualifierTableName = 'tbl_' || qualifierTableName;
    end if;

    if(TG_OP = 'DELETE') then
        perform f_safe_drop_trigger(triggerName, qualifierTableName);
        return old;
    end if;


    cnt = array_upper(nWheres, 1);
    if(cnt <= 0 or cnt > 3) then
        return NULL;
    end if;

    notifyWhere = '';
    for i in 1..cnt
    loop
        select name into nName from notify_where where id = nWheres[i];
        if(nName isnull) then
            return NULL;
        end if;
        notifyWhere := notifyWhere || nName;

        if(cnt > 1 and i < cnt) then
            notifyWhere := notifyWhere || ' or ';
        end if;

    end loop;

    trgFunc = 'notifyqualifier(' || asString(notifyName, false) || ', ' || 
                                    asString(idPosition, false) || ', ' || 
                                    asString(idUnit, false) || ', ' || 
                                    asString(idSearchTemplate, false) || ', ' || 
                                    asString(isAccept, false) || 
                             ')';

    if(TG_OP = 'INSERT') then
        --raise exception '----- %', trgFunc;
        perform f_create_trigger(triggerName, 'after',  notifyWhere, qualifierTableName, trgFunc);
    end if;

    return new;

end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trg_table_notifies_io_objects', 'table_notifies_io_objects');
select f_create_trigger('trg_table_notifies_io_objects', 'after',  'insert or delete', 'table_notifies_io_objects', 'trgcreatenotifytrigger()');

create or replace function trgUpdateNotifyTrigger() returns trigger as
$BODY$
declare
    tableName varchar;
    qualifierTableName varchar;
    trgFunc varchar;
    cnt int4;
    i int4;
    r record;

    oldNotifyName varchar;
    newNotifyName varchar;
    oldTriggerName varchar;
    newTriggerName varchar;
    nName varchar;

    notifyWhere varchar;
    --params and criteria for notify generation
    notifyName varchar; 
    idPosition int4; 
    idUnit int4;
    idSearchTemplate int4;
    isAccept bool;

begin

    tableName = TG_RELNAME;
    if(tableName <> 'table_notifies') then
        return NULL;
    end if;

    if(TG_OP <> 'UPDATE') then
        return NULL;
    end if;

    if(new.notify_where isnull) then
        return NULL;
    end if;

    select count(*) into cnt from table_notifies_io_objects where id_table_notifies = new.id;
    if(cnt <= 0) then
        return new;
    end if;

    oldTriggerName = 'trgasyncnotify_' || lower(old.name);
    newTriggerName = 'trgasyncnotify_' || lower(new.name);


    
    cnt = array_upper(new.notify_where, 1);
    if(cnt <= 0 or cnt > 3) then
        return NULL;
    end if;

    notifyWhere = '';
    for i in 1..cnt
    loop
        select name into nName from notify_where where id = new.notify_where[i];
        if(nName isnull) then
            return NULL;
        end if;

        notifyWhere := notifyWhere || nName;

        if(cnt > 1 and i < cnt) then
            notifyWhere := notifyWhere || ' or ';
        end if;

    end loop;

    notifyName = lower(new.name);
    idPosition = new.id_position;
    idUnit = new.id_unit;
    isAccept = new.is_accept;
    idSearchTemplate = new.id_search_template;
    --nWheres = new.notify_where;

    trgFunc = 'notifyqualifier(' || asString(notifyName, false) || ', ' || 
                                    asString(idPosition, false) || ', ' || 
                                    asString(idUnit, false) || ', ' || 
                                    asString(idSearchTemplate, false) || ', ' || 
                                    asString(isAccept, false) || 
                             ')';


    for r in 
        select io.table_name, io.id from tbl_io_objects io, table_notifies_io_objects t where io.id = t.id_io_objects and t.id_table_notifies = new.id
    loop
        qualifierTableName = r.table_name;
        if(r.id > 300) then
            qualifierTableName = 'tbl_' || qualifierTableName;
        end if;
    
        perform f_safe_drop_trigger(oldTriggerName, qualifierTableName);

        perform f_create_trigger(newTriggerName, 'after',  notifyWhere, qualifierTableName, trgFunc);
    end loop;

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trg_table_notifies', 'table_notifies');
select f_create_trigger('trg_table_notifies', 'after',  'update', 'table_notifies', 'trgupdatenotifytrigger()');


create or replace function trgInsertNotifyLowerTrigger() returns trigger as
$BODY$
declare
    tableName varchar;

begin

    tableName = TG_RELNAME;
    if(tableName <> 'table_notifies') then
        return NULL;
    end if;

    if(TG_OP <> 'UPDATE' and TG_OP <> 'INSERT') then
        return NULL;
    end if;

    if(new.notify_where isnull) then
        return NULL;
    end if;

    new.name = lower(new.name);

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trg_table_notifies_lower', 'table_notifies');
select f_create_trigger('trg_table_notifies_lower', 'before',  'insert or update', 'table_notifies', 'trginsertnotifylowertrigger()');


create or replace function trgDeleteNotifyTrigger() returns trigger as
$BODY$
declare
    tableName varchar;
    qualifierTableName varchar;

    oldNotifyName varchar;
    oldTriggerName varchar;
    cnt int4;
    r record;

begin

    tableName = TG_RELNAME;
    if(tableName <> 'table_notifies') then
        return NULL;
    end if;

    if(TG_OP <> 'DELETE') then
        return NULL;
    end if;

    select count(*) into cnt from table_notifies_io_objects where id_table_notifies = old.id;
    if(cnt <= 0) then
        return old;
    end if;

    oldTriggerName = 'trgasyncnotify_' || lower(old.name);
    

    for r in 
        select io.table_name, io.id from tbl_io_objects io, table_notifies_io_objects t where io.id = t.id_io_objects and t.id_table_notifies = old.id
    loop
        qualifierTableName = r.table_name;
        if(r.id > 300) then
            qualifierTableName = 'tbl_' || qualifierTableName;
        end if;
    
        perform f_safe_drop_trigger(oldTriggerName, qualifierTableName);

    end loop;

    return old;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('trg_table_notifies_delete', 'table_notifies');
select f_create_trigger('trg_table_notifies_delete', 'before',  'delete', 'table_notifies', 'trgdeletenotifytrigger()');
