create or replace function f_sel_attrs_values(int4) returns setof tbl_attrs_values as 
$BODY$
declare 
    idObject alias for $1;
    r tbl_attrs_values%rowtype; 
begin
    if(getPrivilege(getCurrentUser(), 7, 2, true) = false) then 
        raise exception 'You have insufficient permissions to do the operation!'; 
        return; 
    end if;

    if(idObject isnull) then
        for r in 
            select * from tbl_attrs_values av
            where true = getPrivilege(getCurrentUser(), av.id_io_object, 2, true)    
        loop 
            return next r; 
        end loop;
    else
        for r in 
            select * from tbl_attrs_values av
            where av.id_io_object = idObject and true = getPrivilege(getCurrentUser(), av.id_io_object, 2, true)    
        loop 
            return next r; 
        end loop;
    end if;

    return; 
end 
$BODY$ 
language 'plpgsql' security definer;

create or replace function f_ins_attrs_values(int4, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar) returns int4 as
$BODY$
declare
    ii_id_io_object alias for $1;
    ii_id_attr_category alias for $2;
    ivalue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iMeasTime alias for $6;
    iIdObjectSrc alias for $7;
    iIdObjectSrc1 alias for $8;
    iDesc alias for $9;

begin
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    --insert into tbl_attrs_values (id_io_object, id_attr_category, value) values (ii_id_io_object, ii_id_attr_category, ii_value);
    insert into "tbl_attrs_values" (id_io_object, id_attr_category, "value", start_time, stop_time, meas_time, id_io_object_src, id_io_object_src1, is_actual, description) 
    values 
    (ii_id_io_object, ii_id_attr_category, iValue, iStartTime, iStopTime, iMeasTime, iIdObjectSrc, iIdObjectSrc1, true, iDesc);

    return 1;
end
$BODY$
language 'plpgsql' security definer;

create or replace function "f_del_attrs_values"(int4, int4, boolean) returns int4 as 
$BODY$ 
declare 
    ii_id_io_object alias for $1; 
    ii_id_attr_category alias for $2;
    isTemporary alias for $3; 
begin 
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    --���� ��� ��������� ������������ ������ (��� ������ ����� ��������� ��������� ��� �������������� ������, 
    --�������������� ����� ���� ������� ������� � ���, ��� �� ���� ���������)
    if(isTemporary = false) then
        delete from "tbl_attrs_values" where id_io_object = ii_id_io_object and id_attr_category = ii_id_attr_category;
        return 1;
    end if;

    --� ��������� ������ ������ �������� �������� �������� ��� ������������. ��������� ��� � �������
    update "tbl_attrs_values" set is_actual = false, stop_time = current_timestamp where id_io_object = ii_id_io_object and id_attr_category = ii_id_attr_category and is_actual = true;
    
    return 1; 
end 
$BODY$ 
language 'plpgsql' security definer;

create or replace function f_upd_attrs_values(int4, int4, varchar, timestamp, timestamp, timestamp, int4, int4, varchar, int4, int4) returns int4 as
$BODY$
declare
    ii_id_io_object alias for $1;
    ii_id_attr_category alias for $2;
    ivalue alias for $3;
    iStartTime alias for $4;
    iStopTime alias for $5;
    iMeasTime alias for $6;
    iIdObjectSrc alias for $7;
    iIdObjectSrc1 alias for $8;
    iDesc alias for $9;

    old_id_io_object alias for $10;
    old_id_attr_category alias for $11;

    r record;
begin
    if(getPrivilege(getCurrentUser(), 7, 4, true) = false) then raise exception 'You have insufficient permissions to do the operation!'; return 0; end if;

    raise warning 'Desc = %', iDesc;
    
    for r in 
        select * from tbl_attrs_values where id_io_object = old_id_io_object and id_attr_category = old_id_attr_category and is_actual = true
    loop
        if(r.value = iValue and 
           --r.meas_time = iMeasTime and 
           ((r.id_io_object_src isnull and iIdObjectSrc isnull) or r.id_io_object_src = iIdObjectSrc) and 
           ((r.id_io_object_src1 isnull and iIdObjectSrc1 isnull) or r.id_io_object_src1 = iIdObjectSrc1) and 
           ((r.description isnull and iDesc isnull) or r.description = iDesc)
          ) then
            return 1;
        end if;
    end loop;

    --�������� ������� �������� �������� ��� ������������. ��������� ��� � �������
    update "tbl_attrs_values" set is_actual = false, stop_time = current_timestamp where id_io_object = old_id_io_object and id_attr_category = old_id_attr_category and is_actual = true;

    --����� ������� �����, ��� ����� � ����������� ����� ��������
    insert into "tbl_attrs_values" (id_io_object, id_attr_category, "value", start_time, stop_time, meas_time, id_io_object_src, id_io_object_src1, is_actual, description) 
    values 
    (ii_id_io_object, ii_id_attr_category, iValue, current_timestamp, NULL, iMeasTime, iIdObjectSrc, iIdObjectSrc1, true, iDesc);


    return 1;
end
$BODY$
language 'plpgsql' security definer;
