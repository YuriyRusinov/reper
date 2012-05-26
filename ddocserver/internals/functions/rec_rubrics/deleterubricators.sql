create or replace function deleteRubricators(int4, boolean) returns int4 as
$BODY$
declare
    idUser alias for $1;
    bOnlyMyDocs alias for $2;

    idMyDocsRubricator int4;
begin

    select id_rubricator into idMyDocsRubricator from user_rubricator where id_user = idUser;

    delete from access_rubric_table where id_rubric in (select id from getRubricators(idUser, bOnlyMyDocs) where type in (0, 1));

    delete from io_rubricator where id_rubric in (select id from getRubricators(idUser, bOnlyMyDocs) where type in (0, 1));

    delete from user_rubricator where id_user = idUser;

    delete from rubricator where id in (select id from getRubricators(idUser, bOnlyMyDocs) where type in (0, 1));

    delete from rubricator where id = idMyDocsRubricator;

    return 1;

end
$BODY$
language 'plpgsql';
