create or replace function trgTSOAnalyzeQualifiersCheck() returns trigger as
$BODY$
declare
    idObject int4;
    aColor varchar;
    rr record;
    rrr record;
    cnt int4;
begin
/*
    if(new.id < 201) then
        return new;
    end if;

    if(new.record_fill_color = old.record_fill_color) then
        return new;
    end if;

    if(new.unique_id not like('%-io_objects-66') and 
       new.unique_id not like('%-io_objects-67') and 
       new.unique_id not like('%-io_objects-68')) 
    then
        return new;
    end if;

    if(asString(new.record_fill_color, false) = '4278255487') then --çåëåíîå
        update eio_table_1224 set tso_state = 'ÈÑÏÐÀÂÍÎ', "31600-attributes-1247" = 'ÍÅÒ', "31600-attributes-1246" = TSOSetLocalTime("31600-attributes-1265") where id = 1;
        update io_objects set record_fill_color = new.record_fill_color where id = 1224;
    elsif(asString(new.record_fill_color, false) = '4294967040') then 
        update eio_table_1224 set tso_state = 'ÍÅÈÑÏÐÀÂÍÎ', "31600-attributes-1247" = 'ÍÅÒ', "31600-attributes-1246" = TSOSetLocalTime("31600-attributes-1265") where id = 1;
        update io_objects set record_fill_color = new.record_fill_color where id = 1224;
    elsif(asString(new.record_fill_color, false) = '4294923775') then
        update eio_table_1224 set tso_state = 'ÈÑÏÐÀÂÍÎ', "31600-attributes-1247" = 'ÄÀ', "31600-attributes-1246" = TSOSetLocalTime("31600-attributes-1265") where id = 1;
        update io_objects set record_fill_color = new.record_fill_color where id = 1224;
    else
        update eio_table_1224 set tso_state = 'ÍÅÈÑÏÐÀÂÍÎ', "31600-attributes-1247" = 'ÍÅÒ', "31600-attributes-1246" = TSOSetLocalTime("31600-attributes-1265") where id = 1;
        update io_objects set record_fill_color = new.record_fill_color where id = 1224;
    end if;

    for rr in 
        select
            distinct r2.id_io_object as id_obj
        from 
            io_rubricator ior,
            rubricator r,
            rubricator r1,
            rubricator r2
        where 
            ior.id_io_object = new.id
            and ior.id_rubric = r.id
            and r.id_parent = r1.id
            and r1.id_parent = r2.id
    loop
        if(rr.id_obj is null) then 
            continue;
        end if;
        
        update io_objects set record_fill_color = new.record_fill_color where id = rr.id_obj;
        
        update attrs_values set value = asString(new.record_fill_color, false) where id_io_object = rr.id_obj and id_io_attribute in (916, 220) ;
        if(asString(new.record_fill_color, false) = '4278255487') then --çåëåíîå
            update attrs_values set value = 'Èñïðàâíî' where id_io_object = rr.id_obj and id_io_attribute in (915, 219);
        else
            update attrs_values set value = 'Íåèñïðàâíî' where id_io_object = rr.id_obj and id_io_attribute in(915, 219);
        end if;

        for rrr in
            select
                distinct r1.id_io_object as id_obj
            from
                io_rubricator ior,
                rubricator r,
                rubricator r1
            where
                ior.id_io_object = rr.id_obj
                and ior.id_rubric = r.id
                and r.id_parent = r1.id
        loop
            update io_objects set record_fill_color = new.record_fill_color where id = rrr.id_obj;
            update attrs_values set value = asString(new.record_fill_color, false) where id_io_object = rrr.id_obj and id_io_attribute in (916, 220);

            if(asString(new.record_fill_color, false) = '4278255487') then --çåëåíîå
                update attrs_values set value = 'Èñïðàâíî' where id_io_object = rrr.id_obj and id_io_attribute in (915, 219);
            else
                update attrs_values set value = 'Íåèñïðàâíî' where id_io_object = rrr.id_obj and id_io_attribute in (915, 219);
            end if;
        end loop;
        
    end loop;
 */   
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgtsoanalyzequalifiers', 'io_objects');

select f_create_trigger('trgtsoanalyzequalifiers', 'after', 'update', 'io_objects', 'trgtsoanalyzequalifierscheck()');

/*
create trigger trgTSOAnalyzeQualifiers
after update
on io_objects
for each row
execute procedure trgTSOAnalyzeQualifiersCheck();
*/
