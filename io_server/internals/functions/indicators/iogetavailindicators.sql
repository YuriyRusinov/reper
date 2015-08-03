select f_safe_drop_type('h_get_indicator');

create type h_get_indicator as( id_indicator int4, 
                                ind_name varchar,
                                ind_desc varchar,
                                id_indicator_type int4,
                                ind_type_name varchar,
                                id_indicator_parent int4,
                                id_a_type int4,
                                attr_type_name varchar,
                                attr_type_code varchar,
                                id_a_view int4,
                                a_view_name varchar,
                                ind_table_name varchar,
                                ind_column_name varchar,
                                id_ref_attr_type int4);

create or replace function ioGetAvailIndicators(int4) returns setof h_get_indicator as
$BODY$
declare
    idObject alias for $1;
    r h_get_indicator%rowtype;
begin

    for r in 

        select 
            i.id,
            i.name,
            i.description,
            i.id_indicator_type,
            it.name,
            i.id_parent,
            i.id_a_type,
            a.name,
            a.code,
            av.id,
            av.name, 
            i.table_name,
            i.column_name,
            i.id_ref_attr_type
        from 
            indicator i,
            indicator_type it,
            a_types a,
            a_views av

        where 
            i.id_indicator_type = it.id
            and i.id_a_type = a.id
            and a.id_a_view = av.id
            
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';
