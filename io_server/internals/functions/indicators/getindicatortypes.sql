select f_safe_drop_type ('h_get_indicator_type');

create type h_get_indicator_type as (id_indicator_type int4,
                                     indicator_type_name varchar);

create or replace function ioGetIndicatorTypes () returns setof h_get_indicator_type as
$BODY$
declare
    r h_get_indicator_type%rowtype;
begin
    for r in
        select
            it.id,
            it.name
        from
            indicator_type it
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';
