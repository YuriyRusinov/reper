
-------------------------------------------------------
---��������������� �������-----------------------------
-------------------------------------------------------

create or replace function CreateQuery_FromXML(int4, int4) returns varchar as
-- ������������ SELECT-������ �������� �������� ���� FUNC_CODE � ������ XML_ID � ������� XML_QUERY_RESPONSES(ID)
-- ���� ��� ���������� ��� ��� ����� ������, �� ������������ NULL
-- ���� �� � �������, �� ������������ SELECT-������
$BODY$
declare 
    xml_id alias for $1;
    func_code alias for $2;
    xRecord varchar;
begin
    select createXRecordArray(func_code, NULL) into xRecord;
    xRecord := 'select * from ParseXMLResponse(' || xml_id || ') f(id int4, x_level int4,' || xRecord || ');';
    raise WARNING 'xRecord = %', xRecord;
    
    return xRecord;
end;  
$BODY$
language 'plpgsql';

create or replace function getRealID(varchar, int4) returns int4 as
-- ������ ��� ���������� ID �� ������� TabName �� �������� EXTRA_ID
-- ���� ��� ������ EXTRA_ID ��� ������������ ������� ������, �� ���������� NULL
$BODY$
declare tabname alias for $1;
        extraid alias for $2;
        res int4;
begin
  if tabname is null or extraid is null then return null; end if;
  execute 'select id from ' || tabname || ' where extra_id = ' || extraid into res;
  return res;
end;
$BODY$
language 'plpgsql';

-------------------------------------------------------
---�������� ������� ��������---------------------------
-------------------------------------------------------
create or replace function parseEvent(int4, int4, bool) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    bGenCommand alias for $3;

    r record;
    rr record;
    q varchar;
    
    ins_q varchar; --ksa (� ������ �-� ���������� ������ ���� Insert, � �� ����� update)
    upd_q varchar; --ksa (� ������ �-� ���������� ������ ���� Update, � �� ����� update)
    
    p varchar;
    cnt int4;
    xRecord varchar;
    nextid int4;
    personid int4;
    eventid int4;
    deviceid int4;
    devicexml xml;

    bbPU bool;
    bUpdate bool;
begin
    select CreateQuery_FromXML(xml_id, func_code) into q;
    if (q is null) then 
        return null; 
    end if;

    perform startTSOUpdating();
    perform startTSOCreatePerson();

    if(bGenCommand = FALSE) then --���� ������������ ����� � ��������� �������� �� ����, �� �� ���� ������������ ������������
        select f_safe_drop_trigger('trgalarmevents', 'alarm_events_q');
    end if;
    
    create temp table ttt(id_event int4, id_person int4); --������������ ��� ����� ������� � ����������

    for r in execute q 
    loop 
       if r.x_level=0 
       then 
            -- �������� ��������, ����� ������ � �������, ���� ��� ���� �����-�� ����� �� ������ ������. 
            -- ��� �������� �����, ������������ �������� ������ ������������ XML-������ ����� ���������� ���� � ������� ��� �� ID.
            -- ���� ��� �����������, ��� XML �� ����� �����������, �� ��� �������� �� �����
            personId := NULL; 
            
            if r.id_event is null or r.deviceid is null or r.alarm is null -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
            then raise warning '������������ XML-������ �� �������� EVENT_RESPONSE';
                continue;
            end if;     

            deviceId = getRealID('devices',r.deviceid); -- ������� �������� ID �� EXTRA_ID
            if (deviceId is null) then 
                select getNextSeq('devices', 'id') into deviceid; -- ������� ��������� �����
                 -- ������� ��������� ������, ����������� ������ �� ����������� ��������
                 execute 'insert into DEVICES (id, extra_id, name, id_graph_plan) 
                      values ('|| deviceid || ',' || r.deviceid || ',' || quote_literal('newdevice') || ', NULL)';  
                 -- ��������� SVG_IMAGE ��� ����������� �������     
                 execute 'update DEVICES set svg_image = SvgDeviceGenerator (' || deviceId || ') where id = ' || deviceId;

                 perform addTSOUpdating('devices', deviceId, true);
                 
            end if;

            eventId = getRealID('alarm_events',r.id_event); -- ������� �������� ID �� EXTRA_ID
            if (eventId is null) then 
                select getNextSeq('alarm_events', 'id') into eventId; -- ������� ��������� �����
                 -- ������� ��������������� ������, ����������� ������ �� ����������� ��������
  
                ins_q = 'insert into ALARM_EVENTS (id, id_device, extra_id, event_date, message, source, alarm, card_code, pass, direction, video_url) 
                         values (' || eventid || ', ' || deviceId || ', ' || r.id_event;  

                if(r.date is not null) then 
                    ins_q = ins_q || ', ' || quote_literal(r.date); 
                else
                    ins_q = ins_q || ', current_timestamp';
                end if;

                if(r.message is not null) then 
                    ins_q = ins_q || ', ' || quote_literal(r.message); 
                else
                    ins_q = ins_q || ', ' || quote_literal('NewEvent');
                end if;

                if(r.source is not null) then 
                    ins_q = ins_q || ', ' || quote_literal(r.source); 
                else
                    ins_q = ins_q || ', ' || quote_literal('NewSource');
                end if;

                ins_q = ins_q || ', ' || asString(r.alarm, false);

                ins_q = ins_q || ', ' || asString(r.cardcode, true); 
                
                
                ins_q = ins_q || ', ' || asString(r.pass, true); 

                ins_q = ins_q || ', ' || asString(r.direction, true); 

                ins_q = ins_q || ', ' || asString(r.videourl, true); 

                ins_q = ins_q || ')';

--                raise notice '\n % \n', ins_q;
                execute ins_q;

                perform addTSOUpdating('alarm_events', eventId, true);

            else
                -- ������ � �� ����� - ��������� ���������� �� ��������� �������������� �����, ���� ���� ��������   
                 bUpdate = FALSE;
                if r.date is not null then
                    execute 'update ALARM_EVENTS set event_date=' || quote_literal(r.date) || ' where id = ' || eventid || ' and (event_date isnull or event_date <> ' || quote_literal(r.date) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
                if r.message is not null then
                    execute 'update ALARM_EVENTS set message=' || quote_literal(r.message) || ' where id = ' || eventid || ' and (message isnull or message <> ' || quote_literal(r.message) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
                if r.source is not null then
                    execute 'update ALARM_EVENTS set source=' || quote_literal(r.source) || ' where id = ' || eventid || ' and ( source isnull or source <> ' || quote_literal(r.source) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;

                -- ��������!!!! ����� �� ��������, ��� CARD_CODE �� ������ � �������, ������� ������ ������� ��� ��������
                -- ���� �������� ������, �� ����� �������� �������� �� ��� card_code, � ��� ID �� ������� ����.
                if r.cardcode is not null then
                    execute 'update ALARM_EVENTS set card_code=' || quote_literal(r.cardcode) || ' where id = ' || eventid || ' and (card_code isnull or card_code <> ' || quote_literal(r.cardcode) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
                
                if r.pass is not null then
                    execute 'update ALARM_EVENTS set pass=' || r.pass || ' where id = ' || eventid || ' and (pass isnull or pass <> ' || r.pass || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
                if r.direction is not null then
                    execute 'update ALARM_EVENTS set direction=' || quote_literal(r.direction) || ' where id = ' || eventid || ' and (direction isnull or direction <> ' || quote_literal(r.direction) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
                
                if r.videourl is not null then
                execute 'update ALARM_EVENTS set video_url = ' || quote_literal(r.videourl) || ' where id = ' || eventid || ' and (video_url isnull or video_url <> ' || quote_literal(r.videourl) || ')';
                    if(FOUND = TRUE) then
                        bUpdate = TRUE;
                    end if;
                end if;
     
                execute 'update ALARM_EVENTS set id_device=' || deviceid || ' where id = ' || eventid || ' and (id_device isnull or id_device <> ' || deviceid || ')';
                if(FOUND = TRUE) then
                    bUpdate = TRUE;
                end if;

                if(bUpdate = TRUE) then
                    perform addTSOUpdating('alarm_events', eventId, false);
                end if;

            end if; 
            
       end if;

       if (r.x_level=1) then
           if (r.person_id is not null) then -- ���� ������� ����� ���� � �������
               
               personId = getRealID('persons', r.person_id); -- ������� �������� ID �� EXTRA_ID

               if (personId is null) then -- ���� �� ����� ������ ������� ����������� �����, �� ������ ������
                   
                   select getNextSeq('persons', 'id') into personId; -- ������� ��������� ����� ������� � ��������� ��������
                   --execute 'insert into persons (id, extra_id, lastname) values (' || personId || ', ' || r.person_id || ', ''Unnamed person'' )';
                   perform addTSOPersonInfo(personId, 'extra_id', asString(r.person_id, false));

                   --perform addTSOUpdating('persons', personId, true);
               end if;
               
               -- �������� ������ ������� � ID ������� ������ �����, ��� ��� ����� �� ����������� ������� � ����� �� ������
               insert into ttt (id_event, id_person) values(eventId, personId); --����� ������� ��� �� distinct

               --execute 'update ALARM_EVENTS set id_person = ' || personId || ' where id = ' || eventId || ' and id_person <> ' || personId;
               --if(FOUND = TRUE) then
               --    perform addTSOUpdating('alarm_events', eventId, false);
               --end if;
           end if;  
    
           if (personId is null or r.f_name is null or r.f_value is null) then -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
               raise warning '������������ XML-������ � ��������� (personid is null or r.f_name is null or r.f_value is null)';
               continue;
           else
               --bbPU = UpdatePerson(personId, r.f_name, r.f_value); --���� ��� ���� �� ���� ���� ��������� ��������� ��, �� ������ ������� ������ TRUE
                                                                   --� � ���� ������ ����������, ����� � ���������� �������� ������� �� update ��� ������� persons
               --if(bbPU = TRUE) then
               --    perform addTSOUpdating('persons', personId, false);
               --end if;
               perform addTSOPersonInfo(personId, r.f_name, r.f_value);
           end if;
       end if;
    end loop;

    perform stopTSOCreatePerson();

    for rr in select distinct id_event, id_person from ttt
    loop
        update ALARM_EVENTS set id_person = rr.id_person where id = rr.id_event and (id_person isnull or id_person <> rr.id_person);
        if(FOUND = TRUE) then
            perform addTSOUpdating('alarm_events', rr.id_event, false);
        end if;
        
    end loop;
    
    perform stopTSOUpdating();

    drop table ttt;

    if(bGenCommand = FALSE) then
        create trigger trgAlarmEvents
        after insert
        on alarm_events_q
        for each row
        execute procedure trgInsertAlarmEvent();
    end if;


    return p;
end
$BODY$
language 'plpgsql';

--drop function parseDevice(int4, int4);
create or replace function parseFaultDevices(int4, int4) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    r record;
    q varchar;
    
    ins_q varchar;--ksa (� ������ �-� ���������� ������ ���� Insert, � �� ����� update)
    dRealName varchar;
    
    xRecord varchar;
    faultId int4;
    deviceId int4;
    p varchar;
begin
    select CreateQuery_FromXML(xml_id, func_code) into q;
    if q is null then return null; end if;

    --delete from fault_devices_q;
    update fault_devices_q set is_deleted = true;
    delete from fault_devices;
    update dev_object_q set is_ok = true;

    perform setFaultDevicesState(1); --set to OK

    perform startTSOUpdating();
     
    for r in execute q 
    loop
       if r.deviceid is null  -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
       then raise warning '������������ XML-������ �� �������� FAULT_DEVICES';
                  continue;
       end if;     

       deviceId = getRealID('devices', r.deviceid); -- ������� �������� ID �� EXTRA_ID
       
       if (deviceId is null) then 

           select getNextSeq('devices', 'id') into deviceId; -- ������� ��������� �����

           -- ������� ��������� ������, ����������� ������ �� ����������� ��������
           execute 'insert into DEVICES (id, extra_id, name, id_graph_plan) 
                     values (' || deviceId || ', ' || r.deviceId || ', ' || asString(r.devicename, true) || ', NULL)';  

 
           -- ��������� SVG_IMAGE ��� ����������� �������     
           execute 'update DEVICES set svg_image = SvgDeviceGenerator (' || deviceId || ') where id = ' || deviceId;

           perform addTSOUpdating('devices', deviceId, true);
       end if;

       if (r.devicename is not null) then 
           -- ��������� ������ � ������, ���� �������� ������ ������
           execute 'update DEVICES set name = ' || asString(r.devicename, true) || ' where id = ' || deviceId || ' and (name isnull or name <> ' || asString(r.devicename, true) || ')';
           if(FOUND = TRUE) then
               perform addTSOUpdating('devices', deviceId, false);
           end if;
       end if;
       

       select getNextSeq('fault_devices', 'id') into faultId;

       ins_q = 'insert into FAULT_DEVICES (id, id_device, fault_state_name, fault_state_value, reg_datetime) 
                values (' || faultId || ',' || deviceId;

       if(r.faultyStateName is not null) then
           ins_q = ins_q || ', ' || quote_literal(r.faultyStateName);
       else
           ins_q = ins_q || ', ' || quote_literal('����������� ���������');
       end if;

       if(r.faultyStateValue is not null) then
           ins_q = ins_q || ', ' || quote_literal(r.faultyStateValue);
       else
           ins_q = ins_q || ', ' || quote_literal('����������� ���������');
       end if;

       --� ������� ������ ���� �� �������, ������� ����������� ������� ���������
       ins_q = ins_q || ', current_timestamp);';

       execute ins_q;

       perform addTSOUpdating('fault_devices', faultId, true);
       
    end loop;

    perform stopTSOUpdating();

    return p;
end
$BODY$
language 'plpgsql';

create or replace function parseReport(int4, int4) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    r record;
    p varchar;
    q varchar;
    xRecord varchar;
    id_go int4;
    cnt int4;
    nextid int4;
    deviceid int4;
    safeid int4;

    bGuardUpdated bool;
    bDeviceUpdated bool;
begin

    select CreateQuery_FromXML(xml_id, func_code) into q;
    if q is null then return null; end if;

    perform startTSOUpdating();
    perform startTSOGuardObj();
 
    for r in execute q 
    loop

        -- � ���� ����� ��������� ���������� �������� ������
        if (r.x_level = 0) then
        
            if (r.id_safe is null) then -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise notice '������������ XML-������ �� �������� GUARD_OBJECTS';
                continue;
            else 
                safeId := getRealID('guard_objects', r.id_safe); -- ������� �������� ID �� EXTRA_ID
            end if;
    
            -- ���� �������� �������� ID, �� ������������� � ���� ������

            if (safeid is null) then -- ���� ��� ��� ��������� ID 
            
                select getNextSeq('guard_objects', 'id') into safeid; -- ������� ��������� �����
                -- ������� ��������������� ������, ����������� ������ �� ����������� ��������
                execute 'insert into GUARD_OBJECTS (id, extra_id, name, guard_datetime ) 
                         values (' || safeId || ', ' || r.id_safe || ', ' || quote_literal(r.name) || ', ' || asString(r.guarddate, true) || ')';  

                perform addTSOUpdating('guard_objects', safeId, true);
            else
                bGuardUpdated = false;
                
                -- ��������� ���������� �� ��������� �����, ���� ���� ��������   
                if (r.guarddate is not null) then
                    execute 'update GUARD_OBJECTS set guard_datetime = ' || quote_literal(r.guarddate) || ' where id = ' || safeid || ' and (guard_datetime isnull or guard_datetime <> ' || quote_literal(r.guarddate) || ')';
                    if(FOUND = TRUE) then
                        bGuardUpdated = true;
                    end if;
                end if; 
      
                if (r.name is not null) then
                    execute 'update GUARD_OBJECTS set name = ' || asString(r.name, true) || ' where id = ' || safeId || ' and (name isnull or name <> ' || asString(r.name, true) || ')';
                    if(FOUND = TRUE) then
                        bGuardUpdated = true;
                    end if;
                end if;

                perform addTSOUpdating('guard_objects', safeId, false);
            end if;
        end if;


        -- � ���� ����� ��������� ���������� ������� ������
        if (r.x_level = 1) then 
        
            -- ����� ����������� ������ ���� �������� ���������� ��� � ������� ������
            if (safeid is null) then
                raise warning '������������ XML-������ �� �������� GUARD_OBJECTS';
                continue;
            end if;
           
            if (r.device_id is null) then -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise warning '������������ XML-������ �� ����������';
                continue;
            else 
                deviceid := getRealID('devices', r.device_id); -- ������� �������� ID �� EXTRA_ID
            end if;
    
            -- ���� �������� �������� ID, �� ������������� � ���� ������
            if (deviceid is null) then
                select getNextSeq('devices', 'id') into deviceId; -- ������� ��������� �����
                -- ������� ��������������� ������, ����������� ������ �� ����������� ��������
                execute 'insert into DEVICES (id, extra_id, name, id_graph_plan) 
                         values (' || deviceId || ', ' || r.device_id || ', ' || asString(r.device_name, true) || ', NULL)';  
                -- ��������� SVG_IMAGE ��� ����������� �������     
                execute 'update DEVICES set svg_image = SvgDeviceGenerator (' || deviceid || ') where id = ' || deviceid;

                perform addTSOUpdating('devices', deviceId, true);
            end if;
    
            -- ��������� ���������� �� ��������� �������������� �����, ���� ���� ��������   
            if (r.device_name is not null) then
                execute 'update DEVICES set name = ' || asString(r.device_name, true) || ' where id = ' || deviceid || ' and (name isnull or name <> ' || asString(r.device_name, true) || ')';
                if(FOUND = TRUE) then
                    perform addTSOUpdating('devices', deviceId, false);
                end if;
            end if;

            perform addTSOGuardObj(safeId, deviceId);
            -- ������ ������ �������� ������ ||safeid|| - ||deviceid|| ���� ��� ���� (��� �����, ��� ��������� ���� �� ��� ����� ������)
            --execute 'delete from GUARD_OBJECTS_DEVICES where id_guard_objects = ' || safeId || ' and id_device = ' || deviceId;
            -- ������� ����� ������ || safeid || - || deviceid ||
            --execute 'insert into GUARD_OBJECTS_DEVICES (id_guard_objects, id_device) values (' || safeid || ', ' || deviceid || ')';

        end if;

    end loop;

    perform stopTSOUpdating();
    perform stopTSOGuardObj();

    return p;
end
$BODY$
language 'plpgsql';

create or replace function parseGPSchemes(int4, int4) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    r record;
    p varchar;
    q varchar;
    xRecord varchar;
    gplanid int4;
    extraid int4;
    parentid int4;
    cnt int4;
begin

    select CreateQuery_FromXML(xml_id, func_code) into q;
    if q is null then return null; end if;

    perform startTSOUpdating();
    
    for r in execute q 
    loop
        if r.id_scheme is null then -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
            raise warning '������������ XML-������ �� �������� GUARD_OBJECTS';
            continue;
        else 
            if r.parentid is null then -- �������, ��� �������� ������������� ���� ������ ������ ���� �� NULL (���� ��� �������� ������ XML-�������������)
                raise warning '������������ XML-������ �� �������� GUARD_OBJECTS';
                continue;
            else 
                parentid:=getRealID('graph_plans',r.parentid); -- ������� �������� ID �� EXTRA_ID
            end if;
        end if;     

        -- ���� �������� �������� PARENTID, �� ������������� � ���� ������, ���� �� ��� - �� ��������� ��������� ����.����.
        -- ��� ����������� ����� �� �� ���� � ����� ������, ���� ����.���� ��������. ������� � ��������� �������� - ����� ��.
        -- ������������ �� �� ��� �������� �������� ����� ������ ����� -1 �� �������, ��� XML-������������ ����� � ��������.
        -- � ���, ���� �������� �������� ������� ���� ID, �� ������ ����� ��� �������� ��������� � �������. ���� � ������� ��� �� ��������.
        if parentid is null then
            select getNextSeq('graph_plans', 'id') into parentid; -- ������� ��������� �����
            execute 'insert into GRAPH_PLANS (id, extra_id, name, bg_image) 
                     values (' || parentid || ',' || r.parentid || ', ' || quote_literal('�������� ��������') ||', NULL)';                              
            -- ��������� SVG_IMAGE ��� ����������� ���������
            execute 'update GRAPH_PLANS set svg_image = SvgPlanGenerator (' || parentid || ') where id = ' || parentid;

            perform addTSOUpdating('graph_plans', parentId, true); 
        end if;              

        gplanid:=getRealID('graph_plans',r.id_scheme); -- ������� �������� ID �� EXTRA_ID
        if gplanid is null then
            select getNextSeq('graph_plans', 'id') into gplanid; -- ������� ��������� �����
            execute 'insert into GRAPH_PLANS (id, extra_id, id_parent, name, bg_image) 
                     values (' || gplanid || ',' || r.id_scheme || ',' || parentid || ','|| quote_literal('����� ��������') ||', NULL)';
            -- ��������� SVG_IMAGE ��� ����������� ���������
            execute 'update GRAPH_PLANS set svg_image = SvgPlanGenerator ('||gplanid||') where id='||gplanid;

            perform addTSOUpdating('graph_plans', gPlanId, true); 
        end if;              

        -- ��������� ���������� �� ��������� �������������� �����, ���� ���� ��������   
        if r.name is not null then
            execute 'update GRAPH_PLANS set name=' || quote_literal(r.name) || ' where id = ' || gplanid || ' and (name isnull or name <> ' || quote_literal(r.name) || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('graph_plans', gPlanId, false); 
            end if;
        end if;
    
        if r.backgroundimageurl is not null then
            execute 'update GRAPH_PLANS set bg_image=' || quote_literal(r.backgroundimageurl) || ' where id = ' || gplanid || ' and (bg_image isnull or bg_image <> ' || quote_literal(r.backgroundimageurl) || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('graph_plans', gPlanId, false); 
            end if;
        end if;

        if (parentid is not null) then
            execute 'update GRAPH_PLANS set id_parent=' || parentid || ' where id = ' || gplanid || ' and (id_parent isnull or id_parent <> ' || parentId || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('graph_plans', gPlanId, false); 
            end if;
        end if;

         -- ��������� SVG_IMAGE ��� ����������� ���������
         execute 'update GRAPH_PLANS set svg_image = SvgPlanGenerator ('||gplanid||') where id='||gplanid;

    end loop;

    perform stopTSOUpdating();

    return null;
end
$BODY$
language 'plpgsql';

create or replace function parseGPSchemeElements(int4, int4) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    r record;
    rr record;
    p varchar;
    q varchar;
    xRecord varchar;
    nextid int4;
    extraid int4;
    real_id int4;
    deviceid int4;
    gplanid int4;
    shapeid int4;
    shapetypeid int4;
    segmentid int4;
    segmenttypeid int4;
    idParent int4;
    cnt int4;

    currentExtraId int4;
    
    devicexml xml;

    bDeviceUpdated bool;
begin
    select CreateQuery_FromXML(xml_id, func_code) into q;
    if q is null then return null; end if;


    perform startTSOUpdating();

    create temp table ttt (id serial, id_gplan int4, id_device int4);

    for r in execute q 
    loop
       
       
        if (r.x_level = 0) then
            if (r.gplanid is null or r.element_id is null) then -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise warning '������������ XML-������ �� �������� ����.�����';
                continue;
            end if;

            deviceId = NULL;

            if(r.element_type = 'gplan') then
                gPlanId = getRealID('graph_plans',r.element_id); -- ������� �������� ID �� EXTRA_ID

                -- ���� �������� �������� GPLANID, �� ������������� � ���� ������
                if (gplanid is null) then
                     select getNextSeq('graph_plans', 'id') into gplanid; -- ������� ��������� �����

                     idParent = getRealID('graph_plans', r.gplanid);
                     
                     execute 'insert into GRAPH_PLANS (id, extra_id, name, bg_image, id_parent) 
                              values (' || gplanid || ',' || r.element_id || ','|| asString(r.name, true) || ', NULL, ' || asString(idParent, false) || ')';

                     perform addTSOUpdating('graph_plans', gPlanId, true);
                end if;

                insert into ttt(id_gplan, id_device) values(gplanid, NULL);
                currentExtraId = r.element_id;

                continue;
            end if;
            
            gPlanId = getRealID('graph_plans',r.gplanid); -- ������� �������� ID �� EXTRA_ID

            currentExtraId = r.element_id;

            -- ���� �������� �������� GPLANID, �� ������������� � ���� ������
            if (gplanid is null) then
                 select getNextSeq('graph_plans', 'id') into gplanid; -- ������� ��������� �����
                 execute 'insert into GRAPH_PLANS (id, extra_id, name, bg_image) 
                          values (' || gplanid || ',' || r.gplanid || ','|| quote_literal('����� ��������') ||', NULL)';

                 perform addTSOUpdating('graph_plans', gPlanId, true);
            end if;              
            
            deviceId = getRealID('devices',r.element_id); -- ������� �������� ID �� EXTRA_ID
            
            -- ���� �������� �������� DEVICEID, �� ������������� � ���� ������
            if (deviceid is null) then
                 select getNextSeq('devices', 'id') into deviceid; -- ������� ��������� �����
                 execute 'insert into DEVICES (id, extra_id, id_graph_plan, name) 
                          values (' || deviceid || ', ' || r.element_id || ', ' || gPlanId || ', ' || asString(r.name, true) || ')';  
                 perform addTSOUpdating('devices', deviceId, true);
            end if;

            bDeviceUpdated = FALSE;

            if (r.name is not null) then
                execute 'update DEVICES set name=' || asString(r.name, true) || ' where id = ' || deviceId || ' and (name isnull or name <>' || asString(r.name, true) || ')'; 
                if(FOUND = TRUE) then
                    bDeviceUpdated = TRUE;
                end if;
            end if;
            
            if (gplanid is not null) then -- ����� �� GPLANID ����� ������ NOT NULL, �� ���� ������� ���...
                execute 'update DEVICES set id_graph_plan = ' || gplanid || ' where id = ' || deviceid || ' and (id_graph_plan isnull or id_graph_plan <> ' || gPlanId || ')';
                if(FOUND = TRUE) then
                    bDeviceUpdated = TRUE;
                end if;
            end if;
            
            if(bDeviceUpdated = TRUE) then
                perform addTSOUpdating('devices', deviceid, false);
            end if;

            shapeid:=0;

            execute 'delete from shape_segments
                     where id_element_shape in (select id from element_shapes 
                                                where id_device = '||deviceid||')';
            execute 'delete from element_shapes 
                     where id_device = '||deviceid;

            insert into ttt(id_gplan, id_device) values(gPlanId, deviceId);

        end if;  

        if (r.x_level=1) then 

            --�� ������ ������ ����� ��������� ��������, ��� ��� ���������� (�������� ������ ����� � ��������)
            --� ���� ������ ������� ����� ����������
            if(deviceId isnull) then
                 select getNextSeq('devices', 'id') into deviceId; -- ������� ��������� �����
                 execute 'insert into DEVICES (id, extra_id, id_graph_plan, name) 
                          values (' || deviceid || ', ' || asString(currentExtraId, false) || ', ' || gPlanId || ', ' || asString('������ ���������', true) || ')';  
                 perform addTSOUpdating('devices', deviceId, true);

                 insert into ttt(id_gplan, id_device) values(gPlanId, deviceId);
            end if;
            
            if (r.shape_type is null) then -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise warning '������������ XML-������ �� �������� ����.�����';
                continue;
            else 
                execute 'select id from shape_types where name = '||quote_literal(r.shape_type) into shapetypeid;
            end if;

            -- ���� �������� �������� ID, �� ������������� � ���� ������
            if (shapetypeid is null) then
                select getNextSeq('shape_types', 'id') into shapetypeid; -- ������� ��������� �����
                execute 'insert into shape_types (id, name) values ('||shapetypeid||','||quote_literal(r.shape_type)||')';
            end if;     

            segmentid:=0;

            select getNextSeq('element_shapes', 'id') into shapeid; -- ������� ��������� �����
            execute 'insert into element_shapes (id, id_shape_type, id_device) 
                     values (' || shapeid || ', ' || shapetypeid || ', ' || deviceid || ')';
        end if;

        if (r.x_level=2) then
            if (r.segment_type is null) then -- ����� ����������� ������ ���� ��������. ���� ��� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise warning '������������ XML-������ �� �������� ����.�����';
                continue;
            else 
                execute 'select id from segment_types where name = '||quote_literal(r.segment_type) into segmenttypeid;
            end if;

            if (segmenttypeid is null) then
                select getNextSeq('segment_types', 'id') into segmenttypeid; -- ������� ��������� �����
                execute 'insert into segment_types (id, name) values ('||segmenttypeid||','||quote_literal(r.segment_type)||')';
            end if;     

        end if;

        if (r.x_level=3) then
            if (r.x is null or r.y is null) then -- ����� ����������� ������ ���� ��� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
                raise warning '������������ XML-������ �� �������� ����.�����';
                continue;
            else --raise warning '����� �� �������!';
                select getNextSeq('shape_segments', 'id') into segmentid; -- ������� ��������� �����
                execute 'insert into shape_segments (id, id_segment_type, id_element_shape, points) 
                         values ('||segmentid||','||segmenttypeid||','||shapeid||', ARRAY[ POINT(' || r.x::float8 || ',' || r.y::float8 ||')])';
            end if;
        end if;

        --if(deviceid is not null) then
            --execute 'update DEVICES set svg_image = SvgDeviceGenerator (' || deviceId || ') where id = ' || deviceId;
            --execute 'update GRAPH_PLANS set svg_image = SvgPlanGenerator1 (' || deviceId || ') where id = ' || gPlanId;
            --perform addTSOUpdating('devices', deviceId, false);
       --end if;

    end loop;

    for rr in select distinct id_device from ttt where id_device is not null
    loop
        update devices set svg_image =  SvgDeviceGenerator(rr.id_device) where id = rr.id_device;
        perform addTSOUpdating('devices', rr.id_device, false);
    end loop;

    for rr in select distinct id_gplan from ttt
    loop
        update graph_plans set svg_image =  SvgPlanGenerator(rr.id_gplan) where id = rr.id_gplan;
    end loop;

    perform stopTSOUpdating();


    drop table ttt;

    return null;
end
$BODY$
language 'plpgsql';

create or replace function parsePerson(int4, int4) returns varchar as
$BODY$
declare
    xml_id alias for $1;
    func_code alias for $2;
    r record;
    rr record;
    p varchar;
    q varchar;
    nextid int4;
    extraid int4;
    personid int4;
    planid int4;
    cardid int4;
    cardtypeid int4;
    cardstatusid int4;
    cnt int4;

    bPersonCreated bool;
    bPersonUpdated bool;
    bbPU bool;

    qq varchar;

begin
    select CreateQuery_FromXML(xml_id, func_code) into q;
    if q is null then return null; end if;

    perform startTSOUpdating();
    perform startTSOCreatePerson();
    create temp table ttt (id_card int4, id_person int4); --��� ����� ���� ������� � ����������

    for r in execute q 
    loop
        if r.x_level = 0  -- ����� ������������ ���������� � ������� (�������, ���, �����, ���������)
        then 
            -- ����� �� ��������� �������� NULL � �� �������� ����� ����� ������, � ������� �� ��������� ��������� ��� ������ ������� ������ XML-������ � ���������    
            -- �� ������ ������� - ������ ���� 
            if (r.person_id is not null) then -- ���� ������� ����� ���� � �������
        
                personId = getRealID('persons',r.person_id); -- ������� �������� ID �� EXTRA_ID
                if personId is null -- ���� �� ����� ������ ������� ����������� �����, �� ������ ������
                then
                    select getNextSeq('persons', 'id') into personId; -- ������� ��������� ����� ������� � ��������� ��������
                    --execute 'insert into persons (id, extra_id, lastname) values ('||personid||','||r.person_id||',''Unnamed person'')';        

                    --perform addTSOUpdating('persons', personId, true);
                    perform addTSOPersonInfo(personId, 'extra_id', asString(r.person_id, false));
                end if;

            end if;

        if (personid is null or r.f_name is null or r.f_value is null) then -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
            raise warning '������������ XML-������ � ��������� (personid is null or r.f_name is null or r.f_value is null)';
            continue;
        else 
            --bbPU = UpdatePerson(personId, r.f_name, r.f_value); --���� ��� ���� �� ���� ���� ��������� ��������� ��, �� ������ ������� ������ TRUE
                                                                --� � ���� ������ ����������, ����� � ���������� �������� ������� �� update ��� ������� persons
            --if(bbPU = TRUE) then
            --    bPersonUpdated = TRUE;
            --end if;
            perform addTSOPersonInfo(personId, r.f_name, r.f_value);
        end if;    

        --if(bPersonUpdated = TRUE) then 
        --    perform addTSOUpdating('persons', personId, false);
        --end if;
        
    end if;

    if r.x_level=1 -- ����� ������������ ���������� � �����
    then 
        -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
        if (personId is null or r.card_id is null or r.cardtypeid is null or r.cardstatusid is null) then
            raise warning '������������ XML-������ � ��������� (personid is null or r.card_id is null or r.cardtypeid is null or r.cardstatusid is null)';
            continue;
        else 
            cardid=getRealID('access_cards',r.card_id); -- ������� �������� ID �� EXTRA_ID
        end if;

        cardtypeid:=GetRealID('card_types',r.cardtypeid); -- ������� �������� ID �� EXTRA_ID
        if (cardtypeid is null) then
           select getNextSeq('card_types', 'id') into cardtypeid; -- ������� ��������� �����
           execute 'insert into CARD_TYPES (id, extra_id, name) values (' || cardtypeid || ', ' || r.cardtypeid || ', ' || asString(r.cardtypename, true) || ')';
        end if;     
        
        if (r.cardtypename is not null) then
           execute 'update CARD_TYPES set name = ' || asString(r.cardtypename, true) || ' where id = ' || cardtypeid || ' and (name isnull or name <> ' || asString(r.cardtypename, true) || ')';
        end if;

        cardstatusid:=GetRealID('card_statuses',r.cardstatusid); -- ������� �������� ID �� EXTRA_ID
        if cardstatusid is null then
            select getNextSeq('card_statuses', 'id') into cardstatusid; -- ������� ��������� �����
            execute 'insert into CARD_STATUSES (id, extra_id, name) values (' || cardstatusid || ', ' || r.cardstatusid || ', ' || asString(r.cardstatusname, true) || ')';
        end if;     
        
        if r.cardstatusname is not null then
            execute 'update CARD_STATUSES set name = ' || asString(r.cardstatusname, true) || ' where id = ' || cardstatusid || ' and (name isnull or name <> ' || asString(r.cardstatusname, true) || ')';
        end if;

        if (cardid is null) then
            select getNextSeq('access_cards', 'id') into cardid; -- ������� ��������� �����
            execute 'insert into ACCESS_CARDS (id, extra_id, id_card_type, id_card_status, id_person, comment, code) 
                     values (' || cardid || ', ' || r.card_id || ', ' || cardtypeid || ', ' || cardstatusid || ', NULL, ' || asString(r.comment, true) || ', ' || asString(r.cardcode, true) || ' )';

            perform addTSOUpdating('access_cards', cardId, true);
        else 
            execute 'update ACCESS_CARDS set extra_id = ' || r.card_id || ', id_card_type = ' || cardtypeid || ', id_card_status = ' || cardstatusid || ', id_person = NULL ' || 
                     ' where id = ' || cardId;

            perform addTSOUpdating('access_cards', cardId, false);
        end if;

        insert into ttt(id_card, id_person) values(cardId, personId); --����� ��� ������ �� distinct
         
        if r.comment is not null then
            execute 'update ACCESS_CARDS set comment = ' || asString(r.comment, true) || ' where id = ' || cardId || ' and (comment isnull or comment <> ' || asString(r.comment, true) || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('access_cards', cardId, false);
            end if;
        end if;

        if r.cardcode is not null then
            execute 'update ACCESS_CARDS set code = ' || asString(r.cardcode, true) || ' where id = ' || cardId || ' and (code isnull or code <> ' || quote_literal(r.cardcode) || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('access_cards', cardId, false);
            end if;
        end if;
    end if;
       
    if (r.x_level = 2) then -- ����� ������������ ���������� � ����� �������
        
        if (cardid is null or r.plan_id is null) then -- ����� ����������� ������ ���� ��������. ���� �� ��� - �� ����� � �� � ��� ��������, ��������� � ��������� ������.
            raise warning '������������ XML-������ � ��������� (cardid is null or r.plan_id is null)';
            continue;
        else 
            planid=getRealID('access_plan',r.plan_id); -- ������� �������� ID �� EXTRA_ID
        end if;
     
        if planid is null then
            select getNextSeq('access_plan', 'id') into planid; -- ������� ��������� �����
            execute 'insert into ACCESS_PLAN (id, extra_id, name, org_name, org_uid) 
                     select ' || 
                          planid || ',' || 
                          r.plan_id || ',' || 
                          asString(r.plan_name, true) || ', org_name, the_uid from getLocalOrg()';

            qq = 'insert into ACCESS_PLAN (id, extra_id, name, org_name, org_uid) select ' || 
                          planid || ',' || 
                          r.plan_id || ',' || 
                          asString(r.plan_name, true) || ', org_name, the_uid from getLocalOrg()';

            raise warning '%', qq; 


            perform addTSOUpdating('access_plan', planId, true);
        end if;

        if r.plan_name is not null then
            execute 'update ACCESS_PLAN set name = ' || asString(r.plan_name, true) || ' where id = ' || planId || ' and (name isnull or name <> ' || asString(r.plan_name, true) || ')';
            if(FOUND = TRUE) then
                perform addTSOUpdating('access_plan', planId, false);
            end if;
        end if;

        -- ������ ������ �������� ������ ||planid|| - ||cardid|| ���� ��� ���� (��� �����, ��� ��������� ���� �� ��� ����� ������)
        execute 'delete from ACCESS_CARDS_ACCESS_PLAN where id_access_plan = '||planid||' and id_access_cards = '||cardid;  
        -- ������� ����� ������ ||planid|| - ||cardid||
        execute 'insert into ACCESS_CARDS_ACCESS_PLAN (id_access_plan, id_access_cards) values ('||planid||','||cardid||')';  
    end if;

end loop;

    perform stopTSOCreatePerson();--�������� ��� �-� �������� ������, ��������� � stopCreatePerson ���������� addTSOUpdating() ��� ������

    for rr in select distinct id_card, id_person from ttt
    loop
        update ACCESS_CARDS set id_person = rr.id_person where id = rr.id_card and (id_person isnull or id_person <> rr.id_person);
        if(FOUND = TRUE) then
            perform addTSOUpdating('access_cards', rr.id_card, false);
        end if;
        
    end loop;

    drop table ttt;

    perform stopTSOUpdating();


    return null;
end
$BODY$
language 'plpgsql';


