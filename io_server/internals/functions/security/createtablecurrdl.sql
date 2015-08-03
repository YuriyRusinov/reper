create or replace function createTableCurrDl() returns int4 as
$BODY$
declare

begin

    create temp table curr_dl (id_dl int4 not null);
    create temp table notlogging_oper_table (id int4 not null);
    
    return 1;
end
$BODY$
language 'plpgsql';
