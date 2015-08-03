CREATE OR REPLACE FUNCTION xmlinsertiorec()
  RETURNS trigger AS
$BODY$
declare
    idCat int4;
    idCat0 int4;
    idType int4;
    idt int4;
    val varchar;
    query varchar;
    is_receipt boolean;
begin
    --query := E'select jms_schema.message_data.type from jms_schema.message_data inner join jms_schema.in_mes iin on (iin.id_mes=jms_schema.message_data.id and iin.id=' || new.id || ');';
    --execute query into is_receipt;

    --if (not is_receipt) then
        query := E'select jms_schema.message_data.body from jms_schema.message_data inner join jms_schema.in_mes iin on (iin.id_mes=jms_schema.message_data.id and iin.id=' || new.id || ');';
        execute query into val;
        perform ioFromXML (val);
    --end if;

    return new;
end
$BODY$
LANGUAGE plpgsql;

--invoked in readd_zarya22.sql
/*
create trigger trginsertioxml1
after insert
on jms_schema.in_mes
for each row
execute procedure xmlInsertIORec();
*/
