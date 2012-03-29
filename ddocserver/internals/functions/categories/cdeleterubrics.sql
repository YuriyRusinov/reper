create or replace function cDeleteRubrics(int4) returns int4 as
$BODY$
declare
    idCategory alias for $1;
begin
    
    delete from categories_rubrics where id in (select id from cGetRubrics(idCategory) where type in (0, 1));

    return 1;
end
$BODY$
language 'plpgsql';
