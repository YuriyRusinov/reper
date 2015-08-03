-------------------------------------------------------
---“–»√√≈–Õ€≈ ‘”Õ ÷»»----------------------------------
-------------------------------------------------------
create or replace function ChangeAlarm() returns trigger as
$BODY$
declare 
    r record;
    theId int4;
begin
    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;

    select id_ae into theId from alarm_events_q where id_ae = new.id;

    if(theId isnull) then

        insert into alarm_events_q (id_ae,
                                    id_person_p,
                                    event_date,
                                    message,
                                    source,
                                    alarm,
                                    card_code,
                                    pass,
                                    direction,
                                    lastname,
                                    firstname,
                                    sirname,
                                    id_device_p,
                                    dev_name,
                                    id_unit_p,
                                    unit_name,
                                    id_dolzh_p,
                                    dolzh_name,
                                    org_name,
                                    org_uid,
                                    video_url,
                                    for_report, 
                                    is_deleted
) 
                                  select 
                                    AL.id, 
                                    AL.id_person as id_person_p,
                                    AL.event_date,
                                    AL.message,
                                    AL.source,
                                    AL.alarm,
                                    AL.card_code,
                                    AL.pass,
                                    AL.direction,
                                    P.lastname,
                                    P.firstname,
                                    P.sirname,
                                    DEV.id as id_device_p,
                                    DEV.name as dev_name,
                                    U.id as id_unit_p,
                                    U.name as unit_name,
                                    POS.id as id_dolzh_p,
                                    POS.name as dolzh_name,
                                    f.org_name,
                                    f.the_uid,
                                    AL.video_url,
                                    true,
                                    false
                          from alarm_events AL 
                               left join  devices DEV on (al.id_device=dev.id) 
                               left join  persons P on (al.id_person=P.id) 
                               left join tso_units U on (P.id_tso_unit=U.id) 
                               left join tso_positions POS on (P.id_tso_position=POS.id),
                               (select org_name, the_uid from getLocalOrg()) as f                               
                          where AL.id=new.id;     
    end if;

    if(theId is not null) then
        for r in 
                                  select  
                                    AL.id,
                                    AL.id_person as id_person_p,
                                    AL.event_date,
                                    AL.message,
                                    AL.source,
                                    AL.alarm,
                                    AL.card_code,
                                    AL.pass,
                                    AL.direction,
                                    P.lastname,
                                    P.firstname,
                                    P.sirname,
                                    DEV.id as id_device_p,
                                    DEV.name as dev_name,
                                    U.id as id_unit_p,
                                    U.name as unit_name,
                                    POS.id as id_dolzh_p,
                                    POS.name as dolzh_name,
                                    AL.video_url
                          from alarm_events AL 
                               left join  devices DEV on (al.id_device=dev.id) 
                               left join  persons P on (al.id_person=P.id) 
                               left join tso_units U on (P.id_tso_unit=U.id) 
                               left join tso_positions POS on (P.id_tso_position=POS.id)
                          where AL.id = new.id
        loop
            update alarm_events_q set 
                                    id_person_p = r.id_person_p,
                                    event_date = r.event_date,
                                    message = r.message,
                                    source = r.source,
                                    alarm = r.alarm,
                                    card_code = r.card_code,
                                    pass = r.pass,
                                    direction = r.direction,
                                    lastname = r.lastname,
                                    firstname = r.firstname,
                                    sirname = r.sirname,
                                    id_device_p = r.id_device_p,
                                    dev_name = r.dev_name,
                                    id_unit_p = r.id_unit_p,
                                    unit_name = r.unit_name,
                                    id_dolzh_p = r.id_dolzh_p,
                                    dolzh_name = r.dolzh_name,
                                    video_url = r.video_url,
                                    for_report = true,
                                    is_deleted = false
            where id_ae = r.id;
        end loop;
    end if;
    
    return new;
end
$BODY$
language 'plpgsql';

create or replace function ChangeFault() returns trigger as
$BODY$
declare 
    r record;
    oldId int4;
begin

    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;

    select id into oldId from fault_devices_q where id_device_p = new.id_device;
    if(oldId is null) then
    
        insert into fault_devices_q (id_device_p,
                                     dev_name,
                                     fault_state_name,
                                     fault_state_value,
                                     reg_datetime, 
                                     org_name,
                                     org_uid,
                                     is_deleted
                                    )
                                 select  
                                     FD.id_device as id_device_p,
                                     DEV.name as dev_name,
                                     FD.fault_state_name,
                                     FD.fault_state_value,
                                     FD.reg_datetime,
                                     f.org_name,
                                     f.the_uid,
                                     false
                                 from fault_devices FD
                                      left join  devices DEV on (FD.id_device = dev.id),
                                      (select org_name, the_uid from getLocalOrg()) as f
                                 where FD.id=new.id;
    else
        for r in 
            select  
                FD.id_device as id_device_p,
                DEV.name as dev_name,
                FD.fault_state_name,
                FD.fault_state_value,
                FD.reg_datetime,
                f.org_name,
                f.the_uid
            from fault_devices FD
                left join  devices DEV on (FD.id_device = dev.id),
                (select org_name, the_uid from getLocalOrg()) as f
            where FD.id=new.id
        loop
            update fault_devices_q set 
                dev_name = r.dev_name,
                fault_state_name = r.fault_state_name,
                fault_state_value = r.fault_state_value,
                reg_datetime = r.reg_datetime,
                org_name = r.org_name,
                org_uid = r.the_uid,
                is_deleted = false
            where id_device_p = new.id_device;
        end loop;
    end if;

    update dev_object_q set is_ok = false where id_device_p in (select id_device from fault_devices where id = new.id);

    return new;
end
$BODY$
language 'plpgsql';

create or replace function ChangeDevice() returns trigger as
$BODY$
declare 
    r record;
begin
    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;

    update dev_object_q 
    set
        dev_name = new.name,
        device_svg = SvgDeviceGenerator (new.id),
        plan_svg = SvgPlanGenerator1 (new.id) --by id_device
    where 
        id_device_p = new.id;

    update alarm_events_q
    set
        dev_name = new.name
    where id_device_p = new.id;

    return new;
end
$BODY$
language 'plpgsql';

create or replace function ChangeGuard() returns trigger as
$BODY$
declare 
    r record;
begin
    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;

    update dev_object_q 
    set
        guard_obj_name = new.name
    where 
        id_guard_obj_p = new.id;

    return new;
end
$BODY$
language 'plpgsql';


create or replace function ChangeDevObject() returns trigger as
$BODY$
declare 
    r record;
begin
    if(TG_OP = 'INSERT') then

        insert into dev_object_q (id_device_p,
                                   dev_name,
                                   id_guard_obj_p,
                                   guard_obj_name,
                                   device_svg,
                                   plan_svg,
                                   is_ok,
                                   ok_datetime,
                                   org_name,
                                   org_uid
) 
                                  select  
                                   GOD.id_device as id_device_p,
                                   DEV.name as dev_name,
                                   GOD.id_guard_objects as id_guard_obj_p,
                                   GO.name as guard_obj_name,
                                   DEV.svg_image as device_svg,
                                   GP.svg_image as plan_svg,
                                   FD3.is_ok,
                                   FD3.ok_datetime,
                                   f.org_name,
                                   f.the_uid
                                   from guard_objects_devices GOD 
                                        left join  devices DEV on (GOD.id_device=dev.id)
                                        left join  guard_objects GO on (GOD.id_guard_objects=GO.id)
                                        left join  graph_plans GP on (DEV.id_graph_plan=GP.id)
                                        left join
                           -- ‚ ˝ÚÓÏ ÔÓ‰Á‡ÔÓÒÂ ÔÓÎÛ˜‡ÂÏ ÔÓÎˇ is_ok Ë ok_datetime
                                        (select FD1.id, FD1.case as is_ok, FD2.reg_datetime as ok_datetime
                                         from  (select DISTINCT dev.id, CASE WHEN FD.id is null THEN true end from devices DEV
                                                left join fault_devices FD on (DEV.id = FD.id_device)
                                                ) FD1
                                         left join 
                                                (select id_device, max(reg_datetime) as reg_datetime 
                                                 from fault_devices group by id_device
                                                 ) FD2
                                         on (FD1.id=FD2.id_device)) FD3
                                         on (dev.id = FD3.id),
                                         (select org_name, the_uid from getLocalOrg()) as f
                                  where GOD.id_guard_objects=new.id_guard_objects and GOD.id_device=new.id_device;
    end if;
    
    return new;
end
$BODY$
language 'plpgsql';

create or replace function ChangePersons() returns trigger as
$BODY$
declare 
    r record;
    q varchar;
    attCode varchar;

    theId int4;
begin

    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;

    select code into attCode from attributes where id = 181;
    if(attCode isnull) then
        raise exception 'ERROR! Attribute code with id = 181 isNULL!';
        return NULL;
    end if;

    select id_p into theId from personal_q where id_p = new.id;

    if(theId isnull) then

        q = 'insert into personal_q (id_p,
                                     lastname, 
                                     firstname,
                                     sirname, "' || attCode ||
                                     '",
                                     id_dolzh_p,
                                     dolzh_name,
                                     id_unit_p,
                                     unit_name,
                                     comment,
                                     org_name,
                                     org_uid
                                    ) 
                                    select 
                                     P.id,
                                     P.lastname, 
                                     P.firstname,
                                     P.sirname,
                                     AC.array_agg as access_cards,
                                     POS.id as id_dolzh_p,
                                     POS.name as dolzh_name,
                                     U.id as id_unit_p,
                                     U.name as unit_name,
                                     AC.comment,
                                     f.org_name,
                                     f.the_uid
                                     from persons P 
                                          left join  
                                          (select id_person, array_agg(access_cards.id), comment from access_cards, persons where persons.id=access_cards.id_person group by id_person, comment) AC 
                                                                 on (P.id=AC.id_person) 
                                          left join  tso_units U on (P.id_tso_unit=U.id) 
                                          left join  tso_positions POS on (P.id_tso_position=POS.id),
                                          (select org_name, the_uid from getLocalOrg()) as f
                                  where P.id= ' || new.id;     
        --raise notice '%', q;

        execute q;

    end if;

    if(theId is not null) then
        for r in 
                                    select 
                                     P.id,
                                     P.lastname, 
                                     P.firstname,
                                     P.sirname,
                                     AC.array_agg as access_cards,
                                     POS.id as id_dolzh_p,
                                     POS.name as dolzh_name,
                                     U.id as id_unit_p,
                                     U.name as unit_name,
                                     AC.comment
                                     from persons P 
                                          left join  
                                          (select id_person, array_agg(access_cards.id), comment from access_cards, persons where persons.id=access_cards.id_person group by id_person, comment) AC 
                                                                 on (P.id=AC.id_person) 
                                          left join  tso_units U on (P.id_tso_unit=U.id) 
                                          left join  tso_positions POS on (P.id_tso_position=POS.id)
                                  where P.id=new.id
        loop

            q = 'update personal_q set 
                                     lastname = ' || asString(r.lastname, true) || ', 
                                     firstname = ' || asString(r.firstname, true) || ',
                                     sirname = ' || asString(r.sirname, true) || ',
                                     "' || attCode || '" = ' || asString(r.access_cards, true) || ',
                                     id_dolzh_p = ' || asString(r.id_dolzh_p, false) || ',
                                     dolzh_name = ' || asString(r.dolzh_name, true) || ',
                                     id_unit_p = ' || asString(r.id_unit_p, false) || ',
                                     unit_name = ' || asString(r.unit_name, true) || ',
                                     comment = ' || asString(r.comment, true) || '
            where id_p = ' || r.id;

            --raise notice '%', q;
            execute q; 
        
        end loop;
    end if;

        for r in 
                                    select 
                                     P.id,
                                     P.lastname, 
                                     P.firstname,
                                     P.sirname,
                                     AC.array_agg as access_cards,
                                     POS.id as id_dolzh_p,
                                     POS.name as dolzh_name,
                                     U.id as id_unit_p,
                                     U.name as unit_name,
                                     AC.comment
                                     from persons P 
                                          left join  
                                          (select id_person, array_agg(access_cards.id), comment from access_cards, persons where persons.id=access_cards.id_person group by id_person, comment) AC 
                                                                 on (P.id=AC.id_person) 
                                          left join  tso_units U on (P.id_tso_unit=U.id) 
                                          left join  tso_positions POS on (P.id_tso_position=POS.id)
                                  where P.id=new.id
        loop
            update alarm_events_q
            set 
                lastname = r.lastname,
                firstname = r.firstname,
                sirname = r.sirname,
                id_unit_p = r.id_unit_p,
                unit_name = r.unit_name,
                id_dolzh_p = r.id_dolzh_p,
                dolzh_name = r.dolzh_name
            where
                id_person_p = new.id;
        end loop;

    return new;
end
$BODY$
language 'plpgsql';

create or replace function ChangeCards() returns trigger as
$BODY$
declare 
    r record;
    q varchar;
    attCode varchar;
    theId int4;
begin

    if(not (old.is_completed = false and new.is_completed = true)) then
        return new;
    end if;
    
    select code into attCode from attributes where id = 186;
    if(attCode isnull) then
        raise exception 'ERROR! Attribute code with id = 186 is NULL!';
        return NULL;                                            
    end if;

    select id_ac into theId from access_cards_q where id_ac = new.id;
    

    if(theId is NULL) then

        q = 'insert into access_cards_q (
                                    id_ac, 
                                    code,
                                    comment,
                                    id_card_type_p,
                                    card_type_name,
                                    id_card_status_p,
                                    card_status_name, "' || attCode ||
                                    '", org_name,
                                    org_uid
                                       ) 
                                   select 
                                    AC.id,
                                    AC.code,
                                    AC.comment,
                                    AC.id_card_type as id_card_type_p,
                                    CT.name as card_type_name,
                                    AC.id_card_status as id_card_status_p,
                                    CS.name as card_status_name,
                                    ACAP.array_agg as access_plans,
                                    f.org_name,
                                    f.the_uid
                                    from access_cards AC 
                                         left join card_types CT on (AC.id_card_type=CT.id) 
                                         left join card_statuses CS on (AC.id_card_status=CS.id) 
                                         left join 
                                         (select id_access_cards, array_agg(id_access_plan) 
                                          from access_cards_access_plan
                                          group by id_access_cards)
                                         ACAP on (AC.id=ACAP.id_access_cards),
                                         (select org_name, the_uid from getLocalOrg()) as f
                                  where AC.id= ' || new.id;     

        --raise notice '%', q;
        execute q;

    end if;

    if(theId is not null) then
        for r in 
                                   select 
                                    AC.id,
                                    AC.code,
                                    AC.comment,
                                    AC.id_card_type as id_card_type_p,
                                    CT.name as card_type_name,
                                    AC.id_card_status as id_card_status_p,
                                    CS.name as card_status_name,
                                    ACAP.array_agg as access_plans
                                    from access_cards AC 
                                         left join card_types CT on (AC.id_card_type=CT.id) 
                                         left join card_statuses CS on (AC.id_card_status=CS.id) 
                                         left join 
                                         (select id_access_cards, array_agg(id_access_plan) 
                                          from access_cards_access_plan
                                          group by id_access_cards)
                                         ACAP on (AC.id=ACAP.id_access_cards)
                                  where AC.id=new.id
        loop
            q = 'update access_cards_q set 
                                    code = ' || asString(r.code, true) || ',
                                    comment = ' || asString(r.comment, true) || ',
                                    id_card_type_p = ' || asString(r.id_card_type_p, false) || ',
                                    card_type_name = ' || asString(r.card_type_name, true) || ',
                                    id_card_status_p = ' || asString(r.id_card_status_p, false) || ',
                                    card_status_name = ' || asString(r.card_status_name, true) || ', "' ||
                                    attCode || '" = ' || asString(r.access_plans, true) || '
            where id_ac = ' || r.id;

            --raise notice '%', q;

            execute q;

        end loop;

    end if;
    
    return new;
end
$BODY$
language 'plpgsql';




-------------------------------------------------------
---“–»√√≈–€--------------------------------------------
-------------------------------------------------------

select f_safe_drop_trigger('trigalarm', 'alarm_events');

CREATE TRIGGER trigAlarm AFTER UPDATE ON alarm_events
FOR EACH ROW EXECUTE PROCEDURE ChangeAlarm();

select f_safe_drop_trigger('trigfault', 'fault_devices');

CREATE TRIGGER trigFault AFTER UPDATE ON fault_devices
FOR EACH ROW EXECUTE PROCEDURE ChangeFault();

select f_safe_drop_trigger('trigdevice', 'devices');

CREATE TRIGGER trigDevice AFTER UPDATE ON devices
FOR EACH ROW EXECUTE PROCEDURE ChangeDevice();

select f_safe_drop_trigger('trigguard', 'guard_objects');

CREATE TRIGGER trigGuard AFTER UPDATE ON guard_objects
FOR EACH ROW EXECUTE PROCEDURE ChangeGuard();

select f_safe_drop_trigger('trigdevobject', 'guard_objects_devices');

CREATE TRIGGER trigDevObject AFTER INSERT ON guard_objects_devices
FOR EACH ROW EXECUTE PROCEDURE ChangeDevObject();

select f_safe_drop_trigger('trigpersons', 'persons');

CREATE TRIGGER trigPersons AFTER UPDATE ON persons
FOR EACH ROW EXECUTE PROCEDURE ChangePersons();

select f_safe_drop_trigger('trigcards', 'access_cards');

CREATE TRIGGER trigCards AFTER UPDATE ON access_cards
FOR EACH ROW EXECUTE PROCEDURE ChangeCards();

