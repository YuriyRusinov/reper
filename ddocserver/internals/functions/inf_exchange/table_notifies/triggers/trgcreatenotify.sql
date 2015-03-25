/* asyncronous notification for qualifiers */

CREATE OR REPLACE FUNCTION notifyQualifier() RETURNS trigger AS
$BODY$
declare

    tableName varchar;
    recordUniqueId varchar;
    idRecord int8;
    whatHappens int4;

    notifyName varchar;
    payload varchar;

begin

    if(TG_NARGS<=0) then
        return NULL;
    end if;

    notifyName = TG_ARGV[0];    

    tableName := TG_RELNAME;
    if(substr(tableName, 1, 4) = 'tbl_') then
        tableName = substr(tableName, 5);
    end if;

                                                                                      
    if(TG_OP = 'INSERT') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        whatHappens := 1; --new EIO
    elsif(TG_OP = 'UPDATE') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        whatHappens := 2; --updated EIO
    else
        idRecord = old.id;
        recordUniqueId = old.unique_id;
        whatHappens := 3; --updated EIO
    end if;

    payload = createNotify(notifyName, tableName, idRecord, recordUniqueId, whatHappens);
    if(payload isnull) then
        return NULL;
    end if;
                               
    perform pg_notify(notifyName, payload);

    return new;
end
$BODY$ 
language 'plpgsql' SECURITY DEFINER;


create or replace function trgCreateNotifyTrigger() returns trigger as
$BODY$
declare
    tableName varchar;
    notifyName varchar;
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

    select name into notifyName from table_notifies where id = theIdTableNotifies;
    if(notifyName isnull) then
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

    select notify_where into nWheres from table_notifies where id = new.id_table_notifies;
    if(nWheres isnull) then
        return NULL;
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

    trgFunc = 'notifyqualifier(' || quote_literal(notifyName) || ')';

    if(TG_OP = 'INSERT') then
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
    notifyWhere varchar;
    trgFunc varchar;
    cnt int4;
    i int4;
    r record;
    oldNotifyName varchar;
    newNotifyName varchar;
    oldTriggerName varchar;
    newTriggerName varchar;
    nName varchar;
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

    oldTriggerName = 'trgasyncnotify_' || old.name;
    newTriggerName = 'trgasyncnotify_' || new.name;


    
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

    trgFunc = 'notifyqualifier(' || quote_literal(new.name) || ')';


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

