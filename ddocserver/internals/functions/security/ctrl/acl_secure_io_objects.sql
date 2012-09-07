create or replace function f_sel_io_objects(varchar) returns setof tbl_io_objects as
$BODY$
declare 
    uid alias for $1;
    idObject int4;
    r tbl_io_objects%rowtype; 
begin

    if(uid is null) then
        for r in select * from f_sel_io_objects(NULL::int4)
        loop
            return next r;
        end loop;
        return;
    end if;

    select id into idObject from tbl_io_objects where unique_id = uid;
    if(idObject isnull) then
        return;
    end if;

    for r in select * from f_sel_io_objects(idObject)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function f_sel_io_objects_tbl(varchar) returns setof tbl_io_objects as
$BODY$
declare 
    tableName alias for $1;
    idObject int4;
    r tbl_io_objects%rowtype; 
begin
   
    if(tableName is null) then
        for r in select * from f_sel_io_objects(NULL::int4)
        loop
            return next r;
        end loop;
        return;
    end if;


    select id into idObject from tbl_io_objects where table_name = tableName;
    if(idObject isnull) then
        return;
    end if;

    for r in select * from f_sel_io_objects(idObject)
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;

create or replace function f_sel_io_objects(int4) returns setof tbl_io_objects as 
$BODY$
declare 
    idObject alias for $1;
    r tbl_io_objects%rowtype; 
begin
    if(getPrivilege(getCurrentUser(), 7, 2, true) = false) then 
        raise exception 'You have insufficient permissions to do the operation!'; 
        return; 
    end if;

    if(idObject isnull) then
        for r in 
            select * from tbl_io_objects io
            where true = getPrivilege(getCurrentUser(), io.id, 1, true)    
        loop 
            return next r; 
        end loop;
    else
        for r in 
            select * from tbl_io_objects io
            where io.id = idObject and true = getPrivilege(getCurrentUser(), io.id, 1, true)    
        loop 
            return next r; 
        end loop;
    end if;

    return; 
end 
$BODY$ 
language 'plpgsql' security definer;


create or replace function "f_ins_io_objects"(varchar, timestamp, int4, int4, int4, int4, int4, int4, int4, varchar, varchar, varchar, text, bool, timestamp, int4, bool, int8, int8, int4, varchar, int4, varchar) returns int4 as 
$BODY$
declare 
    "ii_unique_id" alias for $1;
    "ii_last_update" alias for $2;
    "ii_id" alias for $3;
    "ii_id_io_category" alias for $4;
    "ii_id_io_state" alias for $5;
    "ii_id_maclabel" alias for $6;
    "ii_author" alias for $7;
    "ii_id_sync_type" alias for $8;
    "ii_id_owner_org" alias for $9;
    "ii_name" alias for $10;
    "ii_table_name" alias for $11;
    "ii_description" alias for $12;
    "ii_information" alias for $13;
    "ii_is_system" alias for $14;
    "ii_insert_time" alias for $15;
    "ii_is_completed" alias for $16;
    "ii_is_global" alias for $17;
    "ii_record_fill_color" alias for $18;
    "ii_record_text_color" alias for $19;
    "ii_id_search_template" alias for $20;
    "ii_ref_table_name" alias for $21;
    "ii_id_io_type" alias for $22;
    "ii_r_icon" alias for $23;
 
begin 
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    insert into "tbl_io_objects" ("unique_id", 
                                  "id", 
                                  "id_io_category", 
                                  "id_io_state", 
                                  "id_maclabel", 
                                  "author", 
                                  "id_sync_type", 
                                  "id_owner_org", 
                                  "name", 
                                  "table_name", 
                                  "description", 
                                  "information", 
                                  "is_system", 
                                  "insert_time",
                                  --"is_completed", 
                                  "is_global", 
                                  "record_fill_color", 
                                  "record_text_color", 
                                  "id_search_template", 
                                  "ref_table_name",
                                  "id_io_type",
                                  "r_icon") 
    values ("ii_unique_id", 
            "ii_id", 
            "ii_id_io_category", 
            "ii_id_io_state", 
            "ii_id_maclabel", 
            "ii_author", 
            "ii_id_sync_type", 
            "ii_id_owner_org", 
            "ii_name", 
            "ii_table_name", 
            "ii_description", 
            "ii_information", 
            "ii_is_system", 
            "ii_insert_time", 
            --"ii_is_completed",
            "ii_is_global", 
            "ii_record_fill_color", 
            "ii_record_text_color", 
            "ii_id_search_template", 
            "ii_ref_table_name",
            "ii_id_io_type",
            "ii_r_icon");
return 1; 
end 
$BODY$ 
language 'plpgsql' security definer;


create or replace function "f_del_io_objects"(int4) returns int4 as 
$BODY$ 
declare 
    ii_id alias for $1; 
begin 
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    delete from "tbl_io_objects" where id = ii_id;
return 1; 
end 
$BODY$ 
language 'plpgsql' security definer;


create or replace function "f_upd_io_objects"(varchar, timestamp, int4, int4, int4, int4, int4, int4, int4, varchar, varchar, varchar, text, bool, timestamp, int4, bool, int8, int8, int4, varchar, int4, varchar) returns int4 as 
$BODY$ 
declare 
"ii_unique_id" alias for $1;
"ii_last_update" alias for $2;
"ii_id" alias for $3;
"ii_id_io_category" alias for $4;
"ii_id_io_state" alias for $5;
"ii_id_maclabel" alias for $6;
"ii_author" alias for $7;
"ii_id_sync_type" alias for $8;
"ii_id_owner_org" alias for $9;
"ii_name" alias for $10;
"ii_table_name" alias for $11;
"ii_description" alias for $12;
"ii_information" alias for $13;
"ii_is_system" alias for $14;
"ii_insert_time" alias for $15;
"ii_is_completed" alias for $16;
"ii_is_global" alias for $17;
"ii_record_fill_color" alias for $18;
"ii_record_text_color" alias for $19;
"ii_id_search_template" alias for $20;
"ii_ref_table_name" alias for $21;
"ii_id_io_type" alias for $22;
"ii_r_icon" alias for $23;
 
begin 
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

update "tbl_io_objects" set 
 "id_io_state" = "ii_id_io_state",
 "id_maclabel" = "ii_id_maclabel",
 "author" = "ii_author",
 "id_sync_type" = "ii_id_sync_type",
 "id_owner_org" = "ii_id_owner_org",
 "name" = "ii_name",
 "table_name" = "ii_table_name",
 "description" = "ii_description",
 "information" = "ii_information",
 "is_system" = "ii_is_system",
 "insert_time" = "ii_insert_time",
 "is_global" = "ii_is_global",
 "is_completed" = "ii_is_completed",
 "record_fill_color" = "ii_record_fill_color",
 "record_text_color" = "ii_record_text_color",
 "id_search_template" = "ii_id_search_template",
 "ref_table_name" = "ii_ref_table_name",
 "id_io_type" = "ii_id_io_type",
 "r_icon" = "ii_r_icon"
 where id = ii_id;
return 1; 
end 
$BODY$ 
language 'plpgsql' security definer;

