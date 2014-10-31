create or replace function insertIncomeXML(int4, int4, int4, varchar) returns int4 as
$BODY$
declare
    idFormat alias for $1;
    interactionType alias for $2;
    idOrganization alias for $3;
    xmlData alias for $4;

    interactionResult int4;
    idRecord int8;
begin

    idRecord = getNextSeq('in_external_queue', 'id');
    if(idRecord <= 0) then
        return -1;
    end if;

    if(xmlData isnull or trim(xmlData) = '') then
        return -1;
    end if;

    if(idFormat isnull or idFormat < 1 or idFormat > 4) then
        return -1;
    end if;

    if(interactionType isnull or interactionType < 1 or interactionType > 3) then
        return -1;
    end if;

    if(idOrganization isnull) then
        return -1;
    end if;

    interactionResult = 1; --new unprocessed data

    insert into in_external_queue (id, id_organization, id_format, in_data, interaction_type, interaction_result)
                            values(idRecord, idOrganization, idFormat, xmlData, interactionType, interactionResult);

    return idRecord;

end
$BODY$
language 'plpgsql';
