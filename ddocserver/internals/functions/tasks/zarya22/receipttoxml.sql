CREATE OR REPLACE FUNCTION receipttoxml(character varying, character varying, character varying, character varying, character varying, character varying, character varying, character varying)
  RETURNS xml AS
$BODY$
declare
    receiptType alias for $1;
    deliveryPriority alias for $2;
    msgKind alias for $3;
    msgCreationDate alias for $4;
    msgCreationTime alias for $5;
    msgSenderUID alias for $6;--UNIQUE_ID of the message on outgoing ACS
    msgReceiverURI alias for $7;--URI of the addressee of the message (msg receiver)
    msgRedirectingNumber alias for $8;


    receiptText varchar;
    receiverName varchar;
    
    xml_str varchar;

    result xml;
begin

    receiptText = E'¬ ответ на ';
    if(msgKind = 'cmd') then
        receiptText = receiptText || 'вашу исход€щую команду';
    elseif (msgKind = 'fdoc') then
        receiptText = receiptText || 'ваше исход€щее формализованное сообщение';
    else 
        receiptText = receiptText || 'ваше исход€щее неформализованное сообщение';
    end if;

    receiptText = receiptText || ' с исход€щим номером ' || asString(msgSenderUID, false);
    receiptText = receiptText || E' подтверждаю его получение.\n';
    receiptText = receiptText || E'ƒоведено до: \n';

    select name into receiverName from shu_dls where uri = msgReceiverURI;
    receiptText = receiptText || asString(receiverName, false);
    
    xml_str := E'<?xml version="1.0" encoding="UTF-8"?>\n<msg>';
    xml_str := xml_str || E'\n';

    xml_str := xml_str || E'\t<passport>\n';
    xml_str := xml_str || E'\t\t<receipt_created>\n';

    xml_str := xml_str || E'\t\t\t<by_automated_control_system>\n';
    xml_str := xml_str || E'\t\t\t\t<with_uri>';
    xml_str := xml_str || E'http://storm.acs.mil';
    xml_str := xml_str || E'</with_uri>\n';
    xml_str := xml_str || E'\t\t\t</by_automated_control_system>\n';

    xml_str := xml_str || E'\t\t\t<as_having>\n';
    xml_str := xml_str || E'\t\t\t\t<type>';
    xml_str := xml_str || receiptType;
    xml_str := xml_str || E'</type>\n';

    xml_str := xml_str || E'\t\t\t\t<creation_date>';
    xml_str := xml_str || to_char(current_date::date, 'DD.MM.YYYY');
    xml_str := xml_str || E'</creation_date>\n';
    xml_str := xml_str || E'\t\t\t\t<creation_time>';
    xml_str := xml_str || to_char(current_time::time, 'HH24:MI:SS');
    xml_str := xml_str || E'</creation_time>\n';
    
    xml_str := xml_str || E'\t\t\t\t<registration_number></registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t<mac_level>1</mac_level>\n';
    xml_str := xml_str || E'\t\t\t\t<delivery_priority>';
    xml_str := xml_str || deliveryPriority;
    xml_str := xml_str || E'</delivery_priority>\n';
    xml_str := xml_str || E'\t\t\t\t<specific_data> <![CDATA[ ]]> </specific_data>\n';
    xml_str := xml_str || E'\t\t\t</as_having>\n';

    xml_str := xml_str || E'\t\t\t<on_msg_which>\n';

    xml_str := xml_str || E'\t\t\t\t<has_kind>';
    xml_str := xml_str || msgKind;
    xml_str := xml_str || E'</has_kind>\n';

    xml_str := xml_str || E'\t\t\t\t<has_creation_date>';
    xml_str := xml_str || msgCreationDate;
    xml_str := xml_str || E'</has_creation_date>\n';

    xml_str := xml_str || E'\t\t\t\t<has_creation_time>';
    xml_str := xml_str || msgCreationTime;
    xml_str := xml_str || E'</has_creation_time>\n';
    
    xml_str := xml_str || E'\t\t\t\t<has_sending_acs_registration_number>';
    xml_str := xml_str || msgSenderUID;
    xml_str := xml_str || E'</has_sending_acs_registration_number>\n';
    
    xml_str := xml_str || E'\t\t\t\t<has_redirecting_acs_registration_number>';
    xml_str := xml_str || asString(msgRedirectingNumber, false);
    xml_str := xml_str || E'</has_redirecting_acs_registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t<has_receiving_acs_registration_number></has_receiving_acs_registration_number>\n';

    xml_str := xml_str || E'\t\t\t\t<has_addressees_uris>\n';
    xml_str := xml_str || E'\t\t\t\t\t<addressee_uri>\n';

    xml_str := xml_str || E'\t\t\t\t\t\t<value>';
    xml_str := xml_str || msgReceiverURI;
    xml_str := xml_str || E'</value>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<delivery_status>0</delivery_status>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<delivery_error_code></delivery_error_code>\n';
    xml_str := xml_str || E'\t\t\t\t\t\t<delivery_error_descr> <![CDATA[ ]]> </delivery_error_descr>\n';

    xml_str := xml_str || E'\t\t\t\t\t</addressee_uri>\n';
    xml_str := xml_str || E'\t\t\t\t</has_addressees_uris>\n';

    xml_str := xml_str || E'\t\t\t</on_msg_which>\n';
    xml_str := xml_str || E'\t\t</receipt_created>\n';
    xml_str := xml_str || E'\t</passport>\n';

    xml_str := xml_str || E'\t<body>\n';

    xml_str := xml_str || E'\t\t<receipt_data>\n';

    xml_str := xml_str || E'\t\t\t<human_readable_text>';
    xml_str := xml_str || E' <![CDATA[ ' || asString(receiptText, false) || E' ]]> ';
    xml_str := xml_str || E'</human_readable_text>\n';

    xml_str := xml_str || E'\t\t\t<formalized_parameters>\n';

    xml_str := xml_str || E'\t\t\t\t<parameters_description>\n';
    xml_str := xml_str || E'\t\t\t\t</parameters_description>\n';
    
    xml_str := xml_str || E'\t\t\t\t<parameters_values>\n';
    xml_str := xml_str || E'\t\t\t\t</parameters_values>\n';

    xml_str := xml_str || E'\t\t\t</formalized_parameters>\n';
    
    xml_str := xml_str || E'\t\t</receipt_data>\n';
    
    xml_str := xml_str || E'\t</body>\n';
    xml_str := xml_str || E'</msg>';

    --raise exception '%', xml_str;
    result := xmlroot (xmlparse(document (xml_str)), version '1.0', standalone no);

    return result;
    
end
$BODY$ 
LANGUAGE plpgsql;
