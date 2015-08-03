select f_safe_drop_type('h_x_get_record_fields');
create type h_x_get_record_fields as(r_ids int4[], r_fields varchar[], r_ref_tables varchar[], r_types int4[]);

create or replace function xGenerateSelectFieldsQuery(int4) returns varchar as
$BODY$
declare
    idCategory alias for $1;
    q varchar;
begin

    q := 'select 
                  a.id as id_attribute, 
                  a.code as a_code, 
                  a.table_name, 
                  a.column_name, 
                  a.ref_column_name as ref_column_name,
                  at.id as id_a_type
              from 
                  attributes a 
                  inner join attrs_categories ac on (ac.id_io_attribute = a.id and ac.id_io_category = ' || idCategory || ') 
                  inner join a_types at on (a.id_a_type = at.id) ';

    return q;
    
end
$BODY$
language 'plpgsql';

create or replace function xGetRecordFields(int4) returns h_x_get_record_fields as
$BODY$
declare
    idCategory alias for $1;
    r record;
    q varchar;

    rFields varchar[];
    rIds int4[];
    rRefTables varchar[];
    rTypes int4[];

    result h_x_get_record_fields;
begin

    rFields = ARRAY[] :: varchar[];
    rIds = ARRAY[] :: int4[];
    rRefTables = ARRAY[] :: varchar[];
    rTypes = ARRAY[] :: int4[];

    q = xGenerateSelectFieldsQuery(idCategory);

    for r in execute q
    loop
        rFields = array_append(rFields, r.a_code);
        rIds = array_append(rIds, r.id_attribute);
        rRefTables = array_append(rRefTables, r.table_name);
        rTypes = array_append(rTypes, r.id_a_type);
    end loop;

    result.r_ids = rIds;
    result.r_fields = rFields;
    result.r_ref_tables = rRefTables;
    result.r_types = rTypes;

    return result;
end
$BODY$
language 'plpgsql';
