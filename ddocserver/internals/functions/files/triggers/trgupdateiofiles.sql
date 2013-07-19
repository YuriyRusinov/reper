create or replace function updateIOUrlsCheck() returns trigger as
$BODY$
declare
    absUrl varchar;
begin
    if(new.uploaded = false) then
        new.hash_sum = NULL;
        return new;
    end if;

    -- in this place new.uploaded allways = TRUE

    if(old.hash_sum isnull) then
    
        absUrl = rGetAbsUrl(new.id);
        new.hash_sum = rGetHashFile(absUrl);

    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgupdateiourls', 'io_urls');

select f_create_trigger('trgupdateiourls', 'before', 'update', 'io_urls', 'updateiourlscheck()');
