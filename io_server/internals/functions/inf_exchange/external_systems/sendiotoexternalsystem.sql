drop function if exists sendIOToExternalSystem(int4, int4, int4);
create or replace function sendIOToExternalSystem(int4, int4, int4, varchar) returns int8 as
$BODY$
declare
    idOrg alias for $1;
    idFormat alias for $2;
    idObject alias for $3;
    xmlData alias for $4;

    idQueue int8;
begin

    select into idQueue getNextSeq('out_external_queue', 'id');
    insert into out_external_queue (id, id_organization, id_format, id_io_object, interaction_type, interaction_result, out_data)
    values (idQueue, idOrg, idFormat, idObject, 2, 1, xmlData);

    return idQueue;

end
$BODY$
language 'plpgsql';
