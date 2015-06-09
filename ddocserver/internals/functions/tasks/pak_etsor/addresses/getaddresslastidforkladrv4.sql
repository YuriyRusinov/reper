create or replace function getAddressLastIdForKladrV4(varchar) returns int8 as
$BODY$
declare
    codeKladr alias for $1;

    kladr varchar;
    lastId int8;

    klCodeLength int4;
begin

    klCodeLength = octet_length(codeKladr);

    if(klCodeLength <> 17 and klCodeLength <> 15) then
        return NULL;
    end if;

    kladr = codeKladr;

    if(klCodeLength = 15) then
        kladr = kladr || '00';
    end if;

    select id into lastId 
    from tbl_eio_table_391 
    where 
        code is not null 
        and code = kladr
        and aolevel <> 8; --houses

    return lastId;

end
$BODY$
language 'plpgsql' security definer;

create or replace function getAddressLastIdForKladrV4(varchar, varchar, varchar, varchar) returns int8 as
$BODY$
declare
    codeKladr alias for $1;
    house alias for $2;
    corps alias for $3;
    bld alias for $4;

    kladr varchar;
    lastId int8;

    klCodeLength int4;
begin

    klCodeLength = octet_length(codeKladr);

    if(klCodeLength <> 17 and klCodeLength <> 15) then
        return NULL;
    end if;

    kladr = codeKladr;

    if(klCodeLength = 15) then
        kladr = kladr || '00';
    end if;

    select id into lastId 
    from tbl_eio_table_391 
    where 
        code is not null 
        and code = kladr
        and (case when (house is not null or corps is not null or bld is not null) then aolevel = 8 else aolevel <> 8 end) --houses
        and (case when house is not null then house_number_str = house else 1=1 end)
        and (case when corps is not null then corps_str = corps else 1=1 end)
        and (case when bld is not null then building = bld else 1=1 end);

    return lastId;

end
$BODY$
language 'plpgsql' security definer;
