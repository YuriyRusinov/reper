CREATE OR REPLACE FUNCTION iofromrecxml(character varying)
  RETURNS boolean AS
$BODY$
declare
    value alias for $1;
    query varchar;

    tdate 	 varchar;
    ttime	 varchar;
    treg 	 varchar;
    tmac 	 varchar;
    tBody 	 varchar;
    torg 	 varchar; 
    tpost	 varchar;
    trank 	 varchar;
    tpost_id	 varchar;
    tpostname	 varchar; 
    tcode 	 varchar;

    idCat int4;
    isFound bool;
    idAttrType int4;
    idAttr int4;
    ioName varchar;
    idOrg int4;
    idObject int4;
    idAttrRes int4;

    addresseeURI varchar;
    attCnt int4;
    attachments varchar;
    fData varchar;
    fData_enc varchar;
    idUrl int4;
    ioUrl int4;

    idDlSender int4;
    idDlReceiver int4;
    idMessage int4;
    msgBody varchar;
    
    isMakeIO boolean;
    r RECORD;

    deliveryPriority varchar;
    msgCreationDate varchar;
    msgCreationTime varchar;
    msgSentDatetime timestamp;
    msgSenderUID varchar;
    msgRedirectingNumber varchar;
    receiptNumbers xml[];
    receiptNumber varchar;
    cnt int4;
    i int4;
    receiptType varchar;

    receiptXML xml;
    xml2Text text;
    muid varchar;

    ttt_aaa varchar;
begin

    isMakeIO := true;
    
    create temp table AAA(id_attr int4, a_val varchar);

    isFound = false;

    -- get sending_acs_registration_number
    tBody :=  '/msg/passport/msg_created/as_having/sending_acs_registration_number/text()';
    select getXMLValue(tBody, value) into msgSenderUID;

    -- get date creating message
    tBody :=  '/msg/passport/msg_created/as_having/creation_date/text()';
    select getXMLValue(tBody, value) into msgCreationDate;

    -- get time creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_time/text()';
    select getXMLValue(tBody, value) into msgCreationTime;

    if(msgCreationDate isnull or trim(msgCreationDate) = '') then
        msgCreationDate = to_char(now()::date, 'DD.MM.YYYY');
    end if;
    if(msgCreationTime isnull or trim(msgCreationTime) = '') then
        msgCreationDate = to_char(now()::time, 'HH24:MI:SS.MS');
    end if;

    msgSentDatetime = to_timestamp(msgCreationDate || ' ' || msgCreationTime, 'DD.MM.YYYY HH24:MI:SS.MS');

    create temp table XXXX (tag_name varchar, the_name varchar, the_title varchar, the_type varchar, the_value varchar);

    ttt_aaa = '';

    for r in select * from getXMLParams(value)
    loop
        isFound = true;

        idAttrType = getXMLAttrType(r.attr_type);
        select aInsert(idAttrType, r.attr_code, r.attr_name, r.attr_title, NULL, NULL, NULL, NULL) into idAttr;

        if(idAttr isnull or idAttr <= 0) then
            drop table AAA;
            drop table XXXX;
            raise exception 'Cannot create attribute';
            return false;
        end if;
        insert into AAA (id_attr, a_val) values (idAttr, r.attr_value);
        ttt_aaa = ttt_aaa || idAttr || ', ';
    end loop;

    drop table XXXX;

    if(isFound = false) then
        raise exception 'Income message does not have any attributes! Cannot create from it new IO!';
        drop table AAA;
        return false;
    else

        select ac.id_io_category into idCat
        from 
           (select ac_ordered.id_io_category, array_agg(ac_ordered.id_io_attribute) as arr
            from 
                (select ac.id_io_category, ac.id_io_attribute from attrs_categories ac order by 1, 2) as ac_ordered, 
                io_categories c
            where 
                ac_ordered.id_io_category = c.id
                and c.is_archived = false
                and c.is_main = true
                and c.id > 300
            group by ac_ordered.id_io_category ) as ac
        where
            ac.arr = (select array_agg(aaa_ordered.id_attr) from (select id_attr from AAA order by 1) as aaa_ordered)
        order by 1
        limit 1;

        raise warning '%', idCat;
       
        if(idCat isnull) then
            --raise exception '%', idCat;
            select cInsert('New income message from JMS', NULL, NULL, 3, NULL, true, NULL, false, 1) into idCat;
            if(idCat isnull or idCat <= 0) then
                drop table AAA;
                raise exception 'Error! Cannot create category for new IO!';
                return false;
            end if;
            update io_categories set name = name || ' - ' || id where id = idCat;

            insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) select idCat, id_attr, NULL, false, false from AAA;
            --perform cSetGlobal(idCat);
        end if;
    end if;


    if (tcode is null or trim(tcode) = '') then
        tcode := 'New income message ' || msgSenderUID;
    end if;

    if(tcode is not null) then
        ioName := tcode;
    else
        ioName = 'New unnamed income message';
    end if;

    select into idOrg id_organization from getlocalorg();

    select into idObject ioinsert (ioName, --name
                                   idCat,  --id_io_category
                                   2/*jupiter*/,  --author
                                   1,  --id_io_state
                                   null::varchar,  --table_name
                                   null::varchar,  --desc
                                   NULL::varchar,  --info
                                   1,  --id_maclabel
                                   NULL::varchar,  --unique_id
                                   1, --id_sync_type
                                   idOrg, --id_owner_org
                                   false, --is_global
                                   null::int4, --id_search_template
                                   null::varchar, --ref_table_name
                                   null::int8, --record_fill_color
                                   null::int8, --record_text_color
                                   1, --id_type
                                   NULL::VARCHAR, --r_icon
                                   NULL::uuid);--uuid_t

    if (idObject is null or idObject <= 0) then
        drop table AAA;
        return false;
    end if;
    
    for r in
        select id_attr, a_val from AAA
    loop
        --select into idAttrRes ioInsertAttr (idObject, r.id_attr, quote_literal (r.a_val)::varchar, current_timestamp::timestamp without time zone, null::timestamp without time zone, null::int4, null::int4, null::varchar);
        select into idAttrRes ioInsertAttr (idObject, r.id_attr, r.a_val, current_timestamp::timestamp without time zone, null::timestamp without time zone, null::int4, null::int4, null::varchar);        
        if (idAttrRes is null or idAttrRes < 0) then
            drop table AAA;
            return null;
        end if;
    end loop;
 
    drop table AAA;

--
--Attached files added here
--

    select writeIOFiles (idObject, value) into idUrl;

--
--insert into message_journal
--

    tbody :=  '/msg/passport/msg_created/to_be_delivered_to_addressees_where/addressee[@number=1]/has_uri/text()';
    select getXMLValue(tBody, value) into addresseeURI;
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(addresseeURI) ||') and sd.id = sdp.id_shu_dls;';
    if(query isnull) then
        idDlReceiver = 4;
    else
        execute query into idDlReceiver;
        if(idDlReceiver isnull) then
            raise exception 'query = %', query;
            idDlReceiver = 4;  --admin
        end if;
    end if;
    

    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
    select getXMLValue(tBody, value) into tpost;
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(tpost) ||') and sd.id = sdp.id_shu_dls;';
    if(query isnull) then
        idDlSender = 4;  --admin
    else
        execute query into idDlSender;
        if(idDlSender isnull) then
            idDlSender = 4;  --admin
        end if;
    end if;

    tbody :=  '/msg/body/unformalized_document_data/human_readable_text/text()';
    select getXMLValue(tBody, value) into msgBody;
    if(msgBody isnull) then
        msgBody := 'new income message';
    end if;


    if(idDlSender isnull or idDlReceiver isnull) then
        raise warning 'Cannot create message in message journal! % (%) --- % (%)', idDlSender, tpost, idDlReceiver, addresseeURI;
        if(getCurrentUser() <> 2)  then --jupiter
            insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
            values(idObject, -10, true, false, false, false, false);
        end if;
        return false;
    end if;

    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
    values(idObject, idDlSender, true, true, true, true, true);

    if(idDlSender <> idDlReceiver) then
        insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
        values(idObject, idDlReceiver, true, true, true, true, true);
    end if;

    select getNextSeq('message_journal', 'id') into idMessage;

    insert into message_journal(id, 
                                id_dl_sender, 
                                id_dl_receiver, 
                                id_io_object, 
                                insert_time, 
                                sent_datetime, 
                                receive_datetime, 
                                read_datetime, 
                                message_body,
                                id_urgency_level,
                                output_number) 
    values
                                    
                               (idMessage, 
                                idDlSender, 
                                idDlReceiver,
                                idObject, 
                                current_timestamp, 
                                msgSentDatetime, 
                                NULL,
                                NULL,
                                msgBody,
                                1,
                                msgSenderUID);

    tbody := '/msg/passport/msg_created/to_be_confirmed_about_delivering_by_receipts_where/receipt/@number';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml)) As date';
    execute query INTO receiptNumbers;

    cnt = array_upper (receiptNumbers, 1);
    
    if(receiptNumbers isnull or cnt isnull or cnt <= 0) then
        return false;
    end if;
    for i in 1..cnt 
    loop
        receiptNumber = receiptNumbers[i]::varchar;
        tbody :=  '/msg/passport/msg_created/to_be_confirmed_about_delivering_by_receipts_where/receipt[@number=' || 
                  asString(receiptNumber, false) || 
                  ']/with_type/text()';
 
        select getXMLValue(tBody, value) into receiptType;
        if(lower(receiptType) <> 'dr.3') then
            continue;
        end if;

        select receiptToXML(receiptType, deliveryPriority, 
                            'ufdoc', msgCreationDate, msgCreationTime, 
                            msgSenderUID, addresseeURI, msgRedirectingNumber) into receiptXML;
        if(receiptXML isnull) then
            continue;
        end if;

        xml2Text = xmlText(receiptXML);

        select unique_id into muid from message_journal where id = idMessage;
        if(muid isnull) then
            continue;
        end if;
        
        --ksa perform jms_schema.add_out_mes (muid || '-receipt', 0, 0, true, xml2Text);
    end loop;    

    return isMakeIO;

end
$BODY$
LANGUAGE plpgsql;
