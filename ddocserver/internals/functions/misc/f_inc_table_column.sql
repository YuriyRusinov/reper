create or replace function f_inc_table_column(varchar, varchar, int8) returns int8 as
$BODY$
declare
    tableName alias for $1;
    columnName alias for $2;
    keyVal alias for $3;

    newVal int8;
    seqName varchar;
    bExist int4;
    ok int4;
    q varchar;
    r record;
    startVal int4;
begin

    seqName = tableName || '_' || columnName || '_' || 'inc_seq';

    bExist = f_is_seq_exist(seqName);
    if(bExist = 0) then

        q = E'select "' || columnName || E'" as startVal from ' || tableName || ' where id = ' || keyVal;
        startVal = -1;
        for r in execute q
        loop
            startVal = r.startVal;
             if(startVal isnull) then
                 startVal = 0;
             end if;
        end loop;

        if(startVal = -1) then
            raise exception 'there is no record with key value = %', keyVal;
            return -1;
        end if; 

        q = E'create sequence "' || seqName || E'" start with ' || startVal+1;
        execute q;

    end if;

    select nextval(seqName) into newVal;

    q = E'update ' || tableName || ' set "' || columnName || '" = ' || newVal || ' where id = ' || keyVal;
    execute q;
  
    return newVal;

end
$BODY$
language 'plpgsql';
