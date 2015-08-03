create or replace function MegaParsing() returns trigger as
$BODY$
declare
    func_name varchar;
    func_code int4;
    r RECORD;
begin

    select 
        x2.id_xml_query_type into func_code
    from 
        xml_query_responses x1 
        inner join xml_queries x2 on (x1.id_xml_query = x2.id)
    where 
        x1.id = new.id;

    select code into func_name
    from xml_query_types 
    where id = func_code;


    if (func_name = 'getNextEventsFullResponse(String filter)') then
        select parseEvent(new.id, func_code, true) into func_name; --при срабатывании этой функции надо генерировать распоряжения
    end if;
  
    if (func_name = 'getFaultyDevicesFullResponse()') then
        select parseFaultDevices(new.id, func_code) into func_name;
    end if;

    if (func_name = 'getSafeReportFullResponse()') then
        select parseReport(new.id, func_code) into func_name;
    end if;

    if (func_name = 'getPersonalReportFullResponse(String filter)') then
        select parsePerson(new.id, func_code) into func_name;
    end if;

    if (func_name = 'getEventReportFullResponse(String filter)') then --при срабатывании этой функции не надо генерировать распоряжения
        select parseEvent(new.id, func_code, false) into func_name;
    end if;

    if (func_name = 'getGPlanSchemesResponse' or func_name = 'getGPlanSchemesFullResponse()') then
        select parseGPSchemes(new.id, func_code) into func_name;
    end if;

    if (func_name = 'getGPlanSchemeElementsResponse' or 
        func_name = 'getNextGPlanSchemeElementsResponse' or 
        func_name = 'getGPlanSchemeElementsFullResponse()') 
    then 
        select parseGPSchemeElements(new.id, func_code) into func_name;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trigparsing', 'xml_query_responses');

CREATE TRIGGER trigParsing AFTER INSERT OR UPDATE ON xml_query_responses
FOR EACH ROW EXECUTE PROCEDURE MegaParsing();
