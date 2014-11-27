create or replace function f_is_seq_exist(varchar) returns int4 as
$BODY$
declare
  iseqname alias for $1;
  tcount int4;
begin
  select count(relname) into tcount from pg_class where relname = iseqname and relkind = 'S';
  return tcount;
end
$BODY$
language 'plpgsql';
