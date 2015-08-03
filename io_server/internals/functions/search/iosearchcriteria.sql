create or replace function insertSearchTemplateType (varchar, varchar, int4) returns int4 as
$BODY$
declare
    iName alias for $1;
    iDesc alias for $2;
    idParent alias for $3;

    idType int4;
begin


    select getNextSeq('search_template_types', 'id') into idType;

    insert into search_template_types (id,
                                       name,
                                       description,
                                       id_parent)
                              values (
                                       idType,
                                       iName,
                                       iDesc,
                                       idParent);

    if (FOUND = false) then
        raise notice 'The search_template_type does not inserted!';
        return -1;
    end if;

    return idType;
end
$BODY$
language 'plpgsql';


create or replace function ioInsertSearchTemplate (varchar, int4) returns int4 as
$BODY$
declare
    searchTemplateName alias for $1;
    idMainGroup alias for $2;

    idSearchTemplate int4;
    idUser int4;
    idParentGroup int4;
begin

    select into idParentGroup id_parent from groups where id=idMainGroup;
    if (idParentGroup is not null) then
        raise notice 'The child group cannot be main template group!';
        return -1;
    end if;

    select getNextSeq('search_templates', 'id') into idSearchTemplate;
    select getCurrentUser() into idUser;

    insert into search_templates (id,
                                  id_group,
                                  name,
                                  author)
                          values (
                                  idSearchTemplate,
                                  idMainGroup,
                                  searchTemplateName,
                                  idUser);

    if (FOUND = false) then
        raise notice 'The search_template does not inserted!';
        return -1;
    end if;

    return idSearchTemplate;
end
$BODY$
language 'plpgsql';

create or replace function ioInsertSearchTemplate (varchar, int4, int4, int4, varchar) returns int4 as
$BODY$
declare
    searchTemplateName alias for $1;
    idMainGroup alias for $2;
    --idAuthor alias for $3;
    idCategory alias for $3;
    idType alias for $4;
    iDescription alias for $5;

    idSearchTemplate int4;
    idUser int4;
    idParentGroup int4;
begin

    select into idParentGroup id_parent from groups where id=idMainGroup;
    if (idParentGroup is not null) then
        raise notice 'The child group cannot be main template group!';
        return -1;
    end if;

    select getNextSeq('search_templates', 'id') into idSearchTemplate;
    select getCurrentUser() into idUser;

    insert into search_templates (id,
                                  id_group,
                                  name,
                                  author,
                                  id_io_category,
                                  id_search_template_type,
                                  creation_datetime,
                                  description)
                          values (
                                  idSearchTemplate,
                                  idMainGroup,
                                  searchTemplateName,
                                  idUser,
                                  idCategory,
                                  idType,
                                  current_timestamp,
                                  iDescription);

    if (FOUND = false) then
        raise notice 'The search_template does not inserted!';
        return -1;
    end if;

    return idSearchTemplate;
end
$BODY$
language 'plpgsql';

create or replace function ioUpdateSearchTemplate (int4, varchar, int4, int4, int4, varchar) returns int4 as
$BODY$
declare
    idSearchTemplate alias for $1;
    stname alias for $2;
    idMainGroup alias for $3;
    idCategory alias for $4;
    idType alias for $5;
    iDescription alias for $6;

    idUser int4;
    idAuthor int4;
begin
    select getCurrentUser() into idUser;
    select into idAuthor author from search_templates where id=idSearchTemplate;
    if (idUser <> 1 and idAuthor <> idUser) then
        raise notice 'only author or db admin can change search template!';
        return -1;
    end if;

    update search_templates 
    set 
        name = stname, 
        id_group = idMainGroup,
        id_io_category = idCategory,
        id_search_template_type = idType,
--        creation_datetime = creationDatetime,
        description = iDescription
    where id=idSearchTemplate;

    if (not FOUND ) then
        raise notice 'The search_template does not updated!';
        return -1;
    end if;

    return idSearchTemplate;
end
$BODY$
language 'plpgsql';

create or replace function ioUpdateSearchTemplate (int4, varchar, int4) returns int4 as
$BODY$
declare
    idSearchTemplate alias for $1;
    stname alias for $2;
    idMainGroup alias for $3;
    

    idUser int4;
    idAuthor int4;
begin
    select getCurrentUser() into idUser;
    select into idAuthor author from search_templates where id=idSearchTemplate;
    if (idUser <> 1 and idAuthor <> idUser) then
        raise notice 'only author or db admin can change search template!';
        return -1;
    end if;

    update search_templates set name=stname, id_group=idMainGroup where id=idSearchTemplate;

    if (not FOUND ) then
        raise notice 'The search_template does not updated!';
        return -1;
    end if;

    return idSearchTemplate;
end
$BODY$
language 'plpgsql';

create or replace function ioDeleteSearchTemplate (int4) returns int4 as
$BODY$
declare
    idSearchTemplate alias for $1;

    cnt int4;
    idUser int4;
    idAuthor int4;
    idMainGroup int4;
    idResdelgr int4;
begin

    select into cnt count(*) from search_templates where id=idSearchTemplate;

    if (cnt = 0) then
        raise notice 'search_template does not contained in database!';
        return -1;
    end if;

    select getCurrentUser() into idUser;
    select into idAuthor author from search_templates where id=idSearchTemplate;
    if (idUser <> 1 and idAuthor <> idUser) then
        raise notice 'only author or db admin can delete search templates!';
        return -1;
    end if;

    select into idMainGroup id_group from search_templates where id=idSearchTemplate;
    update rubricator set id_search_template=null where id_search_template=idSearchTemplate;
    delete from search_templates where id=idSearchTemplate;
    select into idResdelgr ioDeleteGroup (idMainGroup);

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function ioInsertSearchGroup (int4, int4, boolean) returns int4 as
$BODY$
declare
    idParent alias for $1;
    groupOperation alias for $2;
    isNot alias for $3;

    idGroup int4;
begin

    select getNextSeq('groups', 'id') into idGroup;
    insert into groups (id,
                        id_parent,
                        operation,
                        is_not)
                values (
                        idGroup,
                        idParent,
                        groupOperation,
                        isNot);

    if (FOUND = false) then
        raise notice 'group does not inserted!';
        return -1;
    end if;

    return idGroup;
end
$BODY$
language 'plpgsql';

create or replace function ioDeleteGroup (int4) returns int4 as
$BODY$
declare
    idGroup alias for $1;
begin

    delete from groups_criteria where id_group in (select id from getGroups(idGroup, true));

    delete from groups where id in (select id from getGroups(idGroup, true));

    delete from criteria where id in (select c.id from criteria c where c.id not in (select c1.id from criteria c1 inner join groups_criteria gc1 on (gc1.id_criteria=c1.id) ));

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function ioInsertSearchCriterion (int4, varchar, int4, int4, boolean, boolean) returns int4 as
$BODY$
declare
    attrId alias for $1;
    val alias for $2;
    idType alias for $3;
    idOper alias for $4;
    isNot alias for $5;
    caseSensitive alias for $6;

    idCriterion int4;
begin

    select into idCriterion id 
    from 
        criteria c
    where
        c.attr_id = attrId and
        c.value = val and
        c.id_type = idType and
        c.id_operation = idOper and
        c.is_not = isNot and
        c.case_sensitive = caseSensitive;

    if (idCriterion is not null) then
        return idCriterion;
    end if;

    select getNextSeq('criteria', 'id') into idCriterion;
    insert into criteria (id,
                          attr_id,
                          value,
                          id_type,
                          id_operation,
                          is_not,
                          case_sensitive)
                  values (
                          idCriterion,
                          attrId,
                          val,
                          idType,
                          idOper,
                          isNot,
                          caseSensitive);

    if (not FOUND) then
        raise warning 'Criterion does not inserted!';
        return -1;
    end if;

    return idCriterion;
end
$BODY$
language 'plpgsql';

create or replace function ioInsertSearchCriterion (varchar, varchar, int4, int4, boolean, boolean) returns int4 as
$BODY$
declare
    attrUID alias for $1;
    val alias for $2;
    idType alias for $3;
    idOper alias for $4;
    isNot alias for $5;
    caseSensitive alias for $6;

    attrId int4;
begin

    select id into attrId from attributes where unique_id = attrUID;
    if(attrId isnull) then
        return -1;
    end if;

    return ioInsertSearchCriterion(attrId, val, idType, idOper, isNot, caseSensitive);
end
$BODY$
language 'plpgsql';

create or replace function ioInsertCriterionIntoGroup (int4, int4) returns int4 as
$BODY$
declare
    idCriterion alias for $1;
    idGroup alias for $2;

    idRes int4;
begin

    select count(*) as cnt 
    from groups_criteria 
    where 
        id_criteria = idCriterion 
        and id_group=idGroup into idRes;

    if (idRes > 0) then
        raise notice 'criterion already in group!';
        return -1;
    end if;

    insert into groups_criteria (id_criteria, id_group) values (idCriterion, idGroup);

    if (not FOUND) then
        raise notice 'criterion does not inserted!';
        return -1;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';

create or replace function ioClearCriteriaGroup (int4) returns int4 as
$BODY$
declare
    idGroup alias for $1;
begin
    delete from groups_criteria where id_group=idGroup;

    return 1;
end
$BODY$
language 'plpgsql';
