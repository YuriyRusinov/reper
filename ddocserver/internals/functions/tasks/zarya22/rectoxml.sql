create or replace function recToXML (int4, int4, int4, int4[]) returns xml as
$BODY$
declare
    idMsg alias for $1;
    idObject alias for $2;
    idRecord alias for $3;
    idAddrList alias for $4;

    idCategory int4;
    result xml;
    xml_str varchar;
    io_name varchar;

    xml_msg_passport xml;
    xml_form_pars xml;
    --regNumber varchar;
begin

    if ((idObject is null or idRecord is null) and idMsg is null) then
        return null;
    end if;

    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n<msg>';
    xml_str := xml_str || E'\n';

    --
    --в паспорт передается идентификатор ИО, чтобы заполнить тэг doc_name
    --
    select into xml_msg_passport createRecPassport (idObject, idRecord, idMsg, idAddrList, 1);--, regNumber, 1);
    if (xml_msg_passport is null) then
        raise warning 'Invalid passport';
        return null;
    else
        raise warning '%', xml_msg_passport;
    end if;

    if (xml_msg_passport is not null) then
        xml_str := xml_str || xml_msg_passport::varchar;
    end if;
    xml_str := xml_str || E'\n\t';
    xml_str := xml_str || E'<body>\n';

    xml_str := xml_str || E'\t\t<command_data>\n';
    xml_str := xml_str || E'\t\t</command_data>\n';

    xml_str := xml_str || E'\t\t<unformalized_document_data>\n';
    xml_str := xml_str || E'\t\t\t<human_readable_text>\n\t\t\t\t<![CDATA[\n';

    --
    -- если idMsg задан, то в данный тэг записываем message_body
    --
    if(idMsg is not null) then
        select message_body into io_name from message_journal where id = idMsg;
        if(io_name isnull) then
            --
            --если тело сообщения пустое, то пытаемся его получить из названия передаваемого ИО
            --
            select into io_name io.name from io_objects io where io.id = idObject;
        end if;
        --
        --если нет ничего, то наверное нет смысла и передавать сообщение
        --
        if (io_name is null) then
            return null;
        end if;
        xml_str := xml_str || E'\t\t\t\t\t' || io_name;

    end if;
    xml_str := xml_str || E'\n\t\t\t\t]]>\n';

    xml_str := xml_str || E'\t\t\t</human_readable_text>\n';
    
    select into xml_form_pars ufRecToXML (idObject, idRecord);

    if (xml_form_pars is not null) then
        xml_str := xml_str || E'\t\t\t' || xml_form_pars::varchar || E'\t\t\t\n';
    end if;
    xml_str := xml_str || E'\t\t</unformalized_document_data>\n';

    xml_str := xml_str || E'\t\t<formalized_document_data>\n';
    xml_str := xml_str || E'\t\t</formalized_document_data>\n';

    --xml_str := xml_str || E'\t\t<attached_files_set where_files_amnt="0">\n\t\t</attached_files_set>\n';
    xml_str := xml_str || readRecFiles  (idRecord);

    xml_str := xml_str || E'\t</body>\n';
    xml_str := xml_str || E'\n</msg>';

    --raise warning '%', xml_str;
    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);
  
    return result;
end
$BODY$
language 'plpgsql';
