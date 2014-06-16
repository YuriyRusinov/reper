create or replace function sendEIOToExternalSystem(int4, int4, int4, int8) returns int8 as
$BODY$
declare
    idOrg alias for $1;
    idFormat alias for $2;
    idObject alias for $3;
    idRecord alias for $4;

    idQueue int8;
begin

    select into idQueue getNextSeq('out_external_queue', 'id');

    insert into out_external_queue (id, id_organization, id_format, id_io_object, id_entity, interaction_type, interaction_result)
    values (idQueue, idOrg, idFormat, idObject, idRecord, 2, 1);

    return idQueue;

end
$BODY$
language 'plpgsql';
