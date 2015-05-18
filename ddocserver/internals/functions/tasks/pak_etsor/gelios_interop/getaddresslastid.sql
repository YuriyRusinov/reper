create or replace function getAddressLastId(varchar) returns int8 as
$BODY$
declare
    codeKladr alias for $1;
    lastId int8;
    i_regioncode varchar;
    i_autocode varchar;
    i_areacode varchar;
    i_citycode varchar;
    i_ctarcode varchar;
    i_placecode varchar;
    i_streetcode varchar;
    i_extrcode varchar;
    i_sextcode varchar;

    klCodeLength int4;
begin

    klCodeLength = octet_length(codeKladr);
    if(klCodeLength <> 26 and klCodeLength <> 23 and klCodeLength <> 19) then
        return NULL;
    end if;
    
    i_extrcode = NULL;
    i_sextcode = NULL;

    select substring(codeKladr from 1 for 2) into i_regioncode;
    select substring(codeKladr from 3 for 1) into i_autocode;
    select substring(codeKladr from 4 for 3) into i_areacode;
    select substring(codeKladr from 7 for 3) into i_citycode;
    select substring(codeKladr from 10 for 3) into i_ctarcode;
    select substring(codeKladr from 13 for 3) into i_placecode;
    select substring(codeKladr from 16 for 4) into i_streetcode;

    if(klCodeLength > 19) then
        select substring(codeKladr from 20 for 4) into i_extrcode;
        if(klCodeLength > 23) then
            select substring(codeKladr from 24 for 3) into i_sextcode;
        end if;
    end if;

    select id into lastId 
    from tbl_eio_table_391 
    where 
        regioncode = i_regioncode 
        and autocode = i_autocode 
        and areacode = i_areacode
        and citycode = i_citycode
        and ctarcode = i_ctarcode
        and placecode = i_placecode
        and streetcode = i_streetcode
        and (case when i_extrcode is not null then extrcode = i_extrcode else 1=1 end)
        and (case when i_sextcode is not null then sextcode = i_sextcode else 1=1 end);

    return lastId;

end
$BODY$
language 'plpgsql' security definer;
