create or replace function isApplicable(int4, int4) returns bool as
$BODY$
declare
    idSearchTemplate alias for $1;
    idCategory alias for $2;

    cnt int4;
begin

    select count(cr.attr_id) into cnt
    from 
        criteria cr,
        groups_criteria gc,
        groups g,
        search_templates st 
    where 
        g.id in (select id from getGroups(st.id_group, true))
        and g.id = gc.id_group
        and gc.id_criteria = cr.id
        and st.id = idSearchTemplate
        and cr.attr_id not in   (select a.id
                                 from 
                                     attributes a, 
                                     attrs_categories ac 
                                 where 
                                     ac.id_io_category =  idCategory
                                     and ac.id_io_attribute = a.id);

    if(cnt  = 0) then
        return TRUE;
    end if;

    return false;

end
$BODY$
language 'plpgsql';
