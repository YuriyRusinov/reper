create or replace function aggSearchTemplates(int4, int4) returns int4 as
$BODY$
declare
    idTemplate1 alias for $1;
    idTemplate2 alias for $2;

    idTemplate int4;
    topSearchGroup int4;
    sg1 int4;
    sg2 int4;
    idCategory1 int4;
    idCategory2 int4;
    idType1 int4;
    idType2 int4;
    r record;
    aggName varchar;
begin

    if(idTemplate1 isnull or idTemplate2 isnull) then
        return NULL;
    end if;

    topSearchGroup = ioInsertSearchGroup(NULL, 1, FALSE);
    if(topSearchGroup <= 0) then
        return NULL;
    end if;

    for r in 
        select copySearchGroup(id_group) as s1, id_io_category as c1, id_search_template_type as t1  
        from search_templates where id = idTemplate1
    loop
        sg1 = r.s1;
        idCategory1 = r.c1;
        idType1 = r.t1;
    end loop;

    for r in 
        select copySearchGroup(id_group) as s2, id_io_category as c2, id_search_template_type as t2 
        from search_templates where id = idTemplate2
    loop
        sg2 = r.s2;
        idCategory2 = r.c2;
        idType2 = r.t2;
    end loop;

    if(idCategory1 <> idCategory2) then
        raise exception 'Unable aggregate two search templates because of they corresponds to different categories!';
        return NULL;
    end if;

    update groups set id_parent = topSearchGroup where id in(sg1, sg2);

    select 'Объединение (операция "И") запросов: "' || t1.name || '" и "' || t2.name || '"' into aggName
    from search_templates t1, search_templates t2 where t1.id = idTemplate1 and t2.id = idTemplate2;

    idTemplate = ioInsertSearchTemplate(aggName, topSearchGroup, idCategory1, idType1, NULL);
    if(idTemplate <= 0) then
        return NULL;
    end if;

    return idTemplate;
end
$BODY$
language 'plpgsql';
