--тип возвращаемых данных описан в iogetavailindicators.sql

create or replace function getIndicator(int4) returns setof h_get_indicator as
$BODY$
declare
    idIndicator alias for $1;
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
            i.id = idIndicator
--            and i.id = iv.id_indicator
            and i.id_indicator_type = it.id
            and i.id_a_type = a.id
            and a.id_a_view = av.id
            
    loop
        return next r;
    end loop;
    
    return;
end
$BODY$
language 'plpgsql';
