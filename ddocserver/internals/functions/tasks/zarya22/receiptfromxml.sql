CREATE OR REPLACE FUNCTION receiptfromxml(character varying)
  RETURNS boolean AS
$BODY$
declare
    value alias for $1;
    query varchar;
    tbody varchar;

    parentMsgCode varchar;
    parentMsgUID varchar;
    parentMsgId int4;

    receiverURI varchar;
    receiverURIXMLArray xml[];
    receiverCount int4;
    receiverID int4;
    msgID int4;

    receiptCreationDate date;
    receiptCreationTime time;

    i int4;
begin


    tbody := '/msg/passport/receipt_created/on_msg_which/has_kind/text()';
    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)';
    execute query INTO parentMsgCode;
    if(parentMsgCode isnull or (parentMsgCode <> 'ufdoc' and parentMsgCode <> 'fdoc')) then
        return false;
    end if;

    tbody := '/msg/passport/receipt_created/on_msg_which/has_sending_acs_registration_number/text()';
    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)';
    execute query INTO parentMsgUID;
    if(parentMsgUID isnull) then
        raise warning E'ERROR on receiving parent message unique_id for the parsing receipt!\n';
        return false;
    end if;

    query = 'select id from message_journal where unique_id = ' || asString(parentMsgUID, true);
    execute query into parentMsgID;
    if(parentMsgID isnull) then
        raise warning E'ERROR! record in message_journal with given UID does not exist!\n';
        return false;
    end if;


    tbody := '/msg/passport/receipt_created/on_msg_which/has_addressees_uris/addressee_uri/value/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))';
    execute query INTO receiverURIXMLArray;
    if(receiverURIXMLArray isnull) then
        raise warning E'ERROR on parsing receiver addressee URI!\n';
        return false;
    end if;

    select array_upper(receiverURIXMLArray, 1) into receiverCount;
    if(receiverCount <= 0) then
        raise warning E'ERROR on parsing receiver addressee URI!\n';
        return false;
    end if;


    for i in 1 .. receiverCount
    loop
        receiverURI = trim(receiverURIXMLArray[i]::varchar);
        if(receiverURI isnull) then
            continue;
        end if;


        select p.id_position into receiverID from shu_dls_position p, shu_dls d where d.uri = receiverURI and p.id_shu_dls = d.id;
        if(receiverID isnull) then
            continue;
        end if;

        select id into msgID from message_journal where id_dl_receiver = receiverID and id = parentMsgID;
        if(msgID isnull) then
            continue;
        end if;

        tbody := '/msg/passport/receipt_created/as_having/creation_date/text()';
        query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)::date';
        execute query INTO receiptCreationDate;
        if(receiptCreationDate isnull) then
            receiptCreationDate = current_date;
        end if;
        
        tbody := '/msg/passport/receipt_created/as_having/creation_time/text()';
        query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)::time';
        execute query INTO receiptCreationTime;
        if(receiptCreationTime isnull) then
            receiptCreationTime = current_time;
        end if;

        update message_journal 
        set 
            receive_datetime = receiptCreationDate + receiptCreationTime, 
            read_datetime = receiptCreationDate + receiptCreationTime
        where 
            id = msgID;

    end loop;

    return true;
end
$BODY$
LANGUAGE plpgsql;
