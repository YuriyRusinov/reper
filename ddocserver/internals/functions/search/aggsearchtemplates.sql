create or replace function aggSearchTemplates(int4, int4) returns int4 as
$BODY$
declare
    idTemplate1 alias for $1;
    idTemplate2 alias for $2;

    idTemplate int4;
    topSearchGroup int4;
    sg1 int4;
    sg2 int4;
    aggName varchar;
begin

    if(idTemplate1 isnull or idTemplate2 isnull) then
        return NULL;
    end if;

    topSearchGroup = ioInsertSearchGroup(NULL, 1, FALSE);
    if(topSearchGroup <= 0) then
        return NULL;
    end if;

    select copySearchGroup(id_group) into sg1 from search_templates where id = idTemplate1;    
    select copySearchGroup(id_group) into sg2 from search_templates where id = idTemplate2;

    update groups set id_parent = topSearchGroup where id in(sg1, sg2);

    select 'Объединение (операция "И") запросов: "' || t1.name || '" и "' || t2.name || '"' into aggName
    from search_templates t1, search_templates t2 where t1.id = idTemplate1 and t2.id = idTemplate2;

    idTemplate = ioInsertSearchTemplate(aggName, topSearchGroup);
    if(idTemplate <= 0) then
        return NULL;
    end if;

    return idTemplate;
end
$BODY$
language 'plpgsql';
