create or replace function aDeleteAttrAttrs(int4, int4[]) returns int4 as
$BODY$
declare
    idParent alias for $1;
    idsExclude alias for $2;
begin

    if(idParent isnull) then
        return -1;
    end if;

    if(idsExclude isnull) then
        delete from attrs_attrs_values where id_attr_attr in (select id from attrs_attrs where id_attr_parent = idParent);
        delete from rec_attrs_attrs_values where id_attr_attr in (select id from attrs_attrs where id_attr_parent = idParent);
        delete from attrs_attrs where id_attr_parent = idParent;
        return 1;
    end if;

    delete from attrs_attrs_values 
    where 
        id_attr_attr in (select id from attrs_attrs 
                         where 
                             id not in (select id from attrs_attrs 
                                        where id = ANY (idsExclude)) 
                             and id_attr_parent = idParent);

    delete from rec_attrs_attrs_values 
    where 
        id_attr_attr in (select id from attrs_attrs 
                         where 
                             id not in (select id from attrs_attrs 
                                        where id = ANY (idsExclude)) 
                             and id_attr_parent = idParent);

    delete from attrs_attrs where id in (select id from attrs_attrs 
                                         where 
                                             id not in (select id from attrs_attrs 
                                                        where id = ANY (idsExclude)) 
                                             and id_attr_parent = idParent);

    return 1;

end
$BODY$
language 'plpgsql';
