select f_safe_drop_type('h_get_rec_rubrics');
create type h_get_rec_rubrics as (id int8, 
                                  id_parent int8, 
                                  id_record int8, 
                                  name varchar, 
                                  description varchar, 
                                  type int4,
                                  r_icon varchar,
                                  unique_id varchar);

create or replace function recGetRubrics(int8) returns setof h_get_rec_rubrics as
$BODY$
declare 
    idRecord alias for $1;
    rec h_get_rec_rubrics%rowtype;
    rr h_get_rec_rubrics%rowtype;
begin

    for rec in
        select 
            r.id, 
            r.id_parent, 
            r.id_record, 
            r.name, 
            r.description, 
            0,
            r.r_icon,
            r.unique_id
        from record_rubricator r
        where r.id_record = idRecord
        order by r.id
    loop
        return next rec;
        for rr in 
            select * from recGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;
    end loop;
    
    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetSubRubrics(int8) returns setof h_get_rec_rubrics as
$BODY$
declare
    idRubric alias for $1;
    rec h_get_rec_rubrics%rowtype;
    rr h_get_rec_rubrics%rowtype;
begin

    for rec in 
        select r.id, r.id_parent, NULL, r.name, r.description, 1, r.r_icon, r.unique_id
        from record_rubricator r
        where r.id_parent = idRubric
        order by r.id
    loop

        return next rec;

        for rr in 
            select * from recGetSubRubrics(rec.id)
        loop
            return next rr;
        end loop;

        for rr in 
            select * from recGetRubricItems(rec.id)
        loop
            return next rr;
        end loop;
        
    end loop;

    return;    
end
$BODY$
language 'plpgsql';

create or replace function recGetRubricItems(int8) returns setof h_get_rec_rubrics as
$BODY$
declare
    idRubric alias for $1;
    r record;
    rrec h_get_rec_rubrics%rowtype;

    query varchar;
    cquery varchar;
    tname varchar;
    column_name varchar;
    column_desc varchar;
begin

    for r in
        select
            rrs.id_record,
            rrs.id_rubric,
            rrs.r_icon
        from
            rubric_records rrs inner join
            record_rubricator rec
            on (rrs.id_rubric = rec.id and rec.id = idRubric)
        order by 1
    loop
        select into tname getRecordTable (r.id_record);
        cquery := E'select pg_attribute.attname as cname FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT pg_class.oid FROM pg_class LEFT JOIN pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname = \'public\' WHERE pg_class.relname = ' || quote_literal (tname) || E' and pg_attribute.attname=\'name\');';
        execute cquery into column_name;

        cquery := E'select pg_attribute.attname as cname FROM pg_attribute WHERE pg_attribute.attrelid = (SELECT pg_class.oid FROM pg_class LEFT JOIN pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname = \'public\' WHERE pg_class.relname = ' || quote_literal (tname) || E' and pg_attribute.attname=\'description\');';
        execute cquery into column_desc;

        query := E'select t.id, ' || r.id_rubric || E' as id_parent, NULL as id_record, ';
        if (column_name is not null) then
            query := query || E't.name, ';
        else
            query := query || E'NULL, ';
        end if;

        if (column_desc is not null) then
            query := query || E't.description, 2';
        else
            query := query || E'NULL, 2, ' || asString(r.r_icon, true) || ', NULL ' ;
        end if;

        query := query || ' from ' || tname || ' as t where t.id=' || r.id_record;

        for rrec
            in execute query
        loop
            return next rrec;
        end loop;

    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;
