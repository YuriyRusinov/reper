create or replace function trgInExternalQueue() returns trigger as
$BODY$
declare
begin
    perform pg_notify('in_external_queue', asString(new.id, true));

    if(new.id_format = 3) then --shushun
        perform ioFromXML (new.in_data);
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertinexternalqueue', 'in_external_queue');

select f_create_trigger('trginsertinexternalqueue', 'after', 'insert', 'in_external_queue', 'trginexternalqueue()');


create or replace function trgInExternalQueueBefore() returns trigger as
$BODY$
declare
begin

    if(new.interaction_result isnull) then
        new.interaction_result = 1;
    end if;

    if(new.interaction_type isnull) then
        new.interaction_type = 2;
    end if;

    if(new.id_format isnull) then
        new.id_format = 2;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertinexternalqueuebefore', 'in_external_queue');

select f_create_trigger('trginsertinexternalqueuebefore', 'before', 'insert or update', 'in_external_queue', 'trginexternalqueuebefore()');
