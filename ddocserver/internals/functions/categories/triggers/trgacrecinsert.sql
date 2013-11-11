create or replace function aRecInsertCheck() returns trigger as
$BODY$
declare
    idCategory bigint;

    idMainCategory bigint;
    idType bigint;
begin

    idCategory := new.id_io_category;
    select into idType id_io_category_type from io_categories where id=idCategory;
    if (idType = 10) then
    end if;

    return new;
end
$BODY$
language 'plpgsql';
