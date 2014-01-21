create or replace function insertChainsData() returns trigger as
$BODY$
begin
    
    perform pg_notify('chains_data', asString(new.id, true));
    --NOTIFY chains_data;--, new.id;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trginsertchainsdata', 'chains_data');

select f_create_trigger('trginsertchainsdata', 'after', 'insert', 'chains_data', 'insertchainsdata()');


create or replace function insertChainsDataBefore() returns trigger as
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

select f_safe_drop_trigger('trginsertchainsdatabefore', 'chains_data');

select f_create_trigger('trginsertchainsdatabefore', 'before', 'insert', 'chains_data', 'insertchainsdatabefore()');

create or replace function updateChainsDataAfter() returns trigger as
$BODY$
declare
    idReturnedHandler int4;
    r record;
begin

    if(new.return_code is null or new.is_handled <> 1) then
        return new;
    end if;

    if(old.return_code = new.return_code) then
        return new;
    end if;

    select id_handler into idReturnedHandler from chains where id = new.id_chain;
    if(idReturnedHandler isnull) then
        return new;
    end if;

    for r in 
        select lc.id_chains, l.handler_in_data, l.handler_out_data 
        from logistic_chains lc, logistic l 
        where
            lc.id_logistic = l.id
            and l.id_processing_scenario = new.id_processing_scenario
            and l.id_processing_variant = new.id_processing_variant
            and l.id_io_object = new.id_io_object
            and l.id_handler = idReturnedHandler
            and l.return_code = new.return_code
    loop
        insert into chains_data (id_chain, id_io_object, id_parent, id_processing_scenario, id_processing_variant, id_record, what_happens, handler_in_data, handler_out_data, lc_is_parent)
                    values (r.id_chains, new.id_io_object, new.id, new.id_processing_scenario, new.id_processing_variant, new.id_record, 8, r.handler_in_data, r.handler_out_data, false);
    end loop;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgupdatechainsdataafter', 'chains_data');

select f_create_trigger('trgupdatechainsdataafter', 'after', 'update', 'chains_data', 'updatechainsdataafter()');
