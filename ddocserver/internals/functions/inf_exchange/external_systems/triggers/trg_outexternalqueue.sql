create or replace function trgOutExternalQueue() returns trigger as
$BODY$
declare
begin
    perform pg_notify('out_external_queue', asString(new.id, true));

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertoutexternalqueue', 'out_external_queue');

select f_create_trigger('trginsertoutexternalqueue', 'after', 'insert', 'out_external_queue', 'trgoutexternalqueue()');


create or replace function trgOutExternalQueueBefore() returns trigger as
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

select f_safe_drop_trigger('trginsertoutexternalqueuebefore', 'out_external_queue');

select f_create_trigger('trginsertoutexternalqueuebefore', 'before', 'insert or update', 'out_external_queue', 'trgoutexternalqueuebefore()');
