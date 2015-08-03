create or replace function aCreateCopy(int4, varchar) returns int4 as
$BODY$
declare
    idAttributeFrom alias for $1;
    aCode alias for $2;

    idAttribute int4;
begin

    select getNextSeq('attributes', 'id') into idAttribute;
    
    insert into attributes (id, 
                            id_a_type,
                            id_search_template, 
                            id_attr_group, 
                            code, 
                            name, 
                            title, 
                            table_name, 
                            column_name, 
                            ref_column_name, 
                            def_width, 
                            is_system, 
                            id_ref_attr_type)
    select 
                            idAttribute, 
                            a.id_a_type, 
                            a.id_search_template, 
                            a.id_attr_group, 
                            aCode, 
                            a.name, 
                            a.title, 
                            a.table_name, 
                            a.column_name, 
                            a.ref_column_name, 
                            a.def_width, 
                            a.is_system, 
                            a.id_ref_attr_type
    from attributes a
    where a.id = idAttributeFrom;

    return idAttribute;
end
$BODY$
language 'plpgsql';

