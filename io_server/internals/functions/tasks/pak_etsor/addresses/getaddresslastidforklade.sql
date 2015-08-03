create or replace function getAddressLastIdForKlade(varchar) returns int8 as
$BODY$
declare
    codeKlade alias for $1;

    klade varchar;
    lastId int8;

    klCodeLength int4;
begin

    klCodeLength = octet_length(codeKlade);

    if(klCodeLength <> 26 and klCodeLength <> 23 and klCodeLength <> 19) then
        return NULL;
    end if;

    klade = codeKlade;

    if(klCodeLength = 19) then
        klade = klade || '0000000';
    end if;
    if(klCodeLength = 23) then
        klade = klade || '000';
    end if;

    select id into lastId 
    from tbl_eio_table_391 
    where 
        oktmo is not null 
        and oktmo = klade
        and aolevel <> 8; --houses

    return lastId;

end
$BODY$
language 'plpgsql' security definer;

create or replace function getAddressLastIdForKlade(varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    codeKlade alias for $1;
    house alias for $2;
    corps alias for $3;
    bld alias for $4;

    klade varchar;
    lastId int8;

    klCodeLength int4;
begin

    klCodeLength = octet_length(codeKlade);

    if(klCodeLength <> 26 and klCodeLength <> 23 and klCodeLength <> 19) then
        return NULL;
    end if;

    klade = codeKlade;

    if(klCodeLength = 19) then
        klade = klade || '0000000';
    end if;
    if(klCodeLength = 23) then
        klade = klade || '000';
    end if;

    select id into lastId 
    from tbl_eio_table_391 
    where 
        oktmo is not null 
        and oktmo = klade
        and (case when (house is not null or corps is not null or bld is not null) then aolevel = 8 else aolevel <> 8 end) --houses
        and (case when house is not null then house_number_str = house else 1=1 end)
        and (case when corps is not null then corps_str = corps else 1=1 end)
        and (case when bld is not null then building = bld else 1=1 end);

    return lastId;

end
$BODY$
language 'plpgsql' security definer;
