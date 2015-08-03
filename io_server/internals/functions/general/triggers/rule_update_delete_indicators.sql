create or replace function f_del_rec_attrs_values(int8, int4, boolean) returns int4 as 
$BODY$ 
declare 
    ii_id_record alias for $1; 
    ii_id_attr_category alias for $2;
    isTemporary alias for $3; 
begin 
    --if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    --если без поддержки темпоральной модели (это случай когда удал€етс€ полностью сам информационный объект, 
    --соответственно тогда надо –≈јЋ№Ќќ удалить и все, что на него ссылаетс€)
    if(isTemporary = false) then
        delete from rec_attrs_attrs_values where id_rec_attr_value in (select id from rec_attrs_values where id_record = ii_id_record and id_attr_category = ii_id_attr_category);
        delete from "rec_attrs_values" where id_record = ii_id_record and id_attr_category = ii_id_attr_category;
        return 1;
    end if;

    --в противном случае просто помечаем значение атрибута как неактуальное. —охран€ем его в истории
    update "rec_attrs_values" set is_actual = false, stop_time = current_timestamp where id_record = ii_id_record and id_attr_category = ii_id_attr_category and is_actual = true;
    
    return 1; 
end 
$BODY$ 
language 'plpgsql' security definer;

create or replace function f_upd_rec_attrs_values(int8, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar, int8, int4, bool) returns int4 as
$BODY$
declare
    ii_id_record alias for $1;
    ii_id_attr_category alias for $2;
    ivalue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iIdObjectSrc alias for $6;
    iIdObjectSrc1 alias for $7;
    iDesc alias for $8;

    old_id_record alias for $9;
    old_id_attr_category alias for $10;
    isTemporary alias for $11;

    r record;
begin
    --if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    --raise warning 'Desc = %', iDesc;
    
    for r in 
        select * from rec_attrs_values where id_record = old_id_record and id_attr_category = old_id_attr_category and is_actual = true
    loop
        if(r.value = iValue and 
           ((r.id_io_object_src isnull and iIdObjectSrc isnull) or r.id_io_object_src = iIdObjectSrc) and 
           ((r.id_io_object_src1 isnull and iIdObjectSrc1 isnull) or r.id_io_object_src1 = iIdObjectSrc1) and 
           ((r.description isnull and iDesc isnull) or r.description = iDesc)
          ) then
            return 1;
        end if;
    end loop;

    --помечаем текущее значение атрибута как неактуальное. —охран€ем его в истории
    update "rec_attrs_values" set is_actual = false, stop_time = current_timestamp where id_record = old_id_record and id_attr_category = old_id_attr_category and is_actual = true;

    --потом создаем новое, как копию и присваиваем новые значени€
    insert into "rec_attrs_values" (id_record , id_attr_category, "value", start_time, stop_time, id_io_object_src, id_io_object_src1, is_actual, description) 
    values 
    (ii_id_record, ii_id_attr_category, iValue, current_timestamp, NULL, iIdObjectSrc, iIdObjectSrc1, true, iDesc);


    return 1;
end
$BODY$
language 'plpgsql' security definer;


create or replace rule "r_del_rec_attrs_values" as on delete to "rec_attrs_values" do instead select f_del_rec_attrs_values(old.id_record, old.id_attr_category, true);
create or replace rule "r_upd_rec_attrs_values" as on update to "rec_attrs_values" do instead select f_upd_rec_attrs_values(new.id_record, new.id_attr_category, new.value, new.start_time, new.stop_time, new.id_io_object_src, new.id_io_object_src1, new.description, old.id_record, old.id_attr_category);

