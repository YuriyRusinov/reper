create or replace function test_hist (bigint, int4, float8, float8) returns integer as
$BODY$
declare
    numbers alias for $1;
    id_partitition_law alias for $2;
    param1 alias for $3;
    param2 alias for $4;

    i integer;
    val float8;
    id_distr int4;
    query varchar;
begin

    delete from random_values;
    if (id_partitition_law is null or id_partitition_law < 1 or id_partitition_law > 3) then
        id_distr := 1;
    else
        id_distr := id_partitition_law;
    end if;
    perform loadrand();
    for i in 1..numbers
    loop
        if (id_distr = 1) then
            val := param1 + gaussrand(param2);
        elsif (id_distr = 2) then
            val := exprand (param1);
        elsif (id_distr = 3) then
            val := param1 + (param2-param1)*unirand();
        end if;
        query := E'insert into random_values (id_partition_low, value) values (' || id_distr || E',' || val || E')';
        execute query;
    end loop;
    perform droprand();
    return 1;
end
$BODY$
language 'plpgsql';
