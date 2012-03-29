create or replace function createControlPrivTable() returns int4 as
$BODY$
declare

begin

    create temp table control_privileges (is_used bool not null);
    
    return 1;
end
$BODY$
language 'plpgsql';
