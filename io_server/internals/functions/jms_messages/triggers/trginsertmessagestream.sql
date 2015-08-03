create or replace function insertMessageStreamBefore() returns trigger as
$BODY$
begin
    if(new.id_processing_scenario isnull) then
        new.id_processing_scenario = getCurrentProcessingScenario();
    end if;
    if(new.id_processing_variant isnull) then
        new.id_processing_variant = getCurrentProcessingVariant();
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertmessagestreambefore', 'message_streams');

select f_create_trigger('trginsertmessagestreambefore', 'before', 'insert or update', 'message_streams', 'insertmessagestreambefore()');

