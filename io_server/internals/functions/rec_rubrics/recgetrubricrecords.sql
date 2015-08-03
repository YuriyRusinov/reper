select f_safe_drop_type('h_get_rec_rubric_records');
create type h_get_rec_rubric_records as (id_object int4, 
                                         io_name varchar,
                                         id_record int8, 
                                         rr_name varchar, 
                                         unique_id varchar,
                                         uuid_t uuid,
                                         last_update timestamp,
                                         id_io_state int4,
                                         state_name varchar,
                                         r_icon varchar,
                                         record_fill_color int8,
                                         record_text_color int8
                                        );

create or replace function recGetRubricRecords(int4) returns setof h_get_rec_rubric_records as
$BODY$
declare
    idRubric alias for $1;
    r h_get_rec_rubric_records%rowtype;
begin

    for r in 
        select
            io.id,
            io.name,
            q.id,
            q.rr_name,
            q.unique_id,
            q.uuid_t,
            q.last_update,
            q.id_io_state,
            ss.name,
            case when rrs.r_icon is not null then rrs.r_icon else q.r_icon end as r_icon,
            q.record_fill_color,
            q.record_text_color
        from
            rubric_records rrs
            inner join q_base_table q on (rrs.id_rubric = idRubric and q.id = rrs.id_record)
            inner join io_states ss on (q.id_io_state = ss.id)
            inner join pg_class p on (q.tableoid = p.oid)
            inner join tbl_io_objects io on (p.relname = io.table_name or substr(p.relname, 5) = io.table_name)
        order by 1, 2
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetRubricRecords(int4, int8[], int8[]) returns setof h_get_rec_rubric_records as
$BODY$
declare
    idRubric alias for $1;
    addRecords alias for $2;
    delRecords alias for $3;

    r h_get_rec_rubric_records%rowtype;
begin

    for r in 
        select
            io.id,
            io.name,
            q.id,
            q.rr_name,
            q.unique_id,
            q.uuid_t,
            q.last_update,
            q.id_io_state,
            ss.name,
            case when rrs.r_icon is not null then rrs.r_icon else q.r_icon end as r_icon,
            q.record_fill_color,
            q.record_text_color
        from
            rubric_records rrs
            right join q_base_table q on (rrs.id_rubric = idRubric and q.id = rrs.id_record)
            inner join io_states ss on (q.id_io_state = ss.id)
            inner join pg_class p on (q.tableoid = p.oid)
            inner join tbl_io_objects io on (p.relname = io.table_name or substr(p.relname, 5) = io.table_name)
        where
            (  
                rrs.id_record is not null 
                or ( case when addRecords is not null then q.id = ANY (addRecords) else 1=0 end)
            )
            and 
                case when delRecords is not null then q.id <> ALL (delRecords) else 1=1 end
        order by 1, 2
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';

create or replace function recGetRecordsInfo(int8[]) returns setof h_get_rec_rubric_records as
$BODY$
declare
    idRecords alias for $1;
    r h_get_rec_rubric_records%rowtype;
begin

    for r in 
        select
            io.id,
            io.name,
            q.id,
            q.rr_name,
            q.unique_id,
            q.uuid_t,
            q.last_update,
            q.id_io_state,
            ss.name,
            q.r_icon,
            q.record_fill_color,
            q.record_text_color
        from
            q_base_table q
            inner join io_states ss on (q.id_io_state = ss.id)
            inner join pg_class p on (q.tableoid = p.oid)
            inner join tbl_io_objects io on (p.relname = io.table_name or substr(p.relname, 5) = io.table_name)
        where q.id = ANY(idRecords)
        order by 1, 2
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
