create or replace function copySearchGroup(int4) returns int4 as
$BODY$
declare
    idSrcGroup alias for $1;

    idDstGroup int4;
    idGroup int4;
    idParent int4;
    idCriterion int4;

    r record;
    rr record;
begin

    create temp table ttt(old_id int4, new_id int4);

    for r in 
        select 
            id, id_parent, operation, is_not, type 
        from getGroups(idSrcGroup, true)
    loop
        idGroup = getNextSeq('groups', 'id');

        insert into ttt (old_id, new_id) values (r.id, idGroup);

        if(r.id_parent is not null) then
            select new_id into idParent from ttt where old_id = r.id_parent;
        else 
            idParent = NULL;
            idDstGroup = idGroup;
        end if;

        insert into groups(id, id_parent, operation, is_not) values(idGroup, idParent, r.operation, r.is_not);

        for rr in
            select id, attr_id, "value", id_type, id_operation, case_sensitive, is_not
            from ioGetCriterionFromGroup(r.id)
        loop

            idCriterion = ioInsertSearchCriterion(rr.attr_id, rr.value, rr.id_type, rr.id_operation, rr.is_not, rr.case_sensitive);

            perform ioInsertCriterionIntoGroup(idCriterion, idGroup);
        end loop;
        
    end loop;

    drop table ttt;

    return idDstGroup;

end
$BODY$
language 'plpgsql';
