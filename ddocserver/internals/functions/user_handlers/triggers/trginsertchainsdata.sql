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
