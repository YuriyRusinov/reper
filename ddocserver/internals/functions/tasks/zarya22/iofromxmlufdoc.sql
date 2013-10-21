CREATE OR REPLACE FUNCTION iofromxmlufdocsimple(character varying)
  RETURNS boolean AS
$BODY$
declare
    value alias for $1;
    query varchar;

    tdate 	 varchar;
    ttime	 varchar;
    treg 	 varchar;
    tmac 	 varchar;
    tbody 	 varchar;
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
begin

    isMakeIO := true;
    
    create temp table AAA(id_attr int4, a_val varchar);

-- get date creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_date/text()';
    --query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As date';
    select getXMLValue(tBody, value) into msgCreationDate;
    --execute query INTO msgCreationDate;
    insert into AAA (id_attr, a_val) values (233, msgCreationDate);

-- get time creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_time/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As time';
--    execute query INTO msgCreationTime;
    select getXMLValue(tBody, value) into msgCreationTime;
    insert into AAA (id_attr, a_val) values (234, msgCreationTime);

-- get sending_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/sending_acs_registration_number/text()';
    --query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As reg';
    --execute query INTO msgSenderUID;
    select getXMLValue(tBody, value) into msgSenderUID;

-- get redirecting_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/redirecting_acs_registration_number/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As reg';
--    execute query INTO msgRedirectingNumber;
    select getXMLValue(tBody, value) into msgRedirectingNumber;
    --insert into XXX (id_attr, a_val) values (233, tdate);

-- get mac level message
    tbody :=  '/msg/passport/msg_created/as_having/mac_level/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As mac_level';
--    execute query INTO tmac;
    select getXMLValue(tBody, value) into tmac;
    insert into AAA (id_attr, a_val) values (235, tmac);

    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_uri/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_acs';
--    execute query INTO torg;
    select getXMLValue(tBody, value) into torg;

    query =  'select id from shu_acs where uri = trim(' || asString(torg, true) ||')';
    execute query into torg;
    insert into AAA (id_attr, a_val) values (225, torg);


    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_working_mode/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As mode';
--    execute query INTO torg;
    select getXMLValue(tBody, value) into torg;    
    insert into AAA (id_attr, a_val) values (231, torg);



-- get organization uri
    tbody :=  '/msg/passport/msg_created/by_organization_unit/with_uri/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_org';
--    execute query INTO torg;
    select getXMLValue(tBody, value) into torg;
    query =  'select id from shu_orgs where uri = trim(' || asString(torg, true)||')';
    execute query into torg;
    insert into AAA (id_attr, a_val) values (226, torg);

-- get post uri
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_post';
--    execute query INTO tpost;
    select getXMLValue(tBody, value) into tpost;
    query =  'select id from shu_dls where uri = trim(' || asString(tpost, true)||')';
    execute query into tpost;
    insert into AAA (id_attr, a_val) values (228, tpost);


-- get rank id
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_rank_id/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As rank_id';
--    execute query INTO trank;
    select getXMLValue(tBody, value) into trank;
    query =  'select id from ranks where code = trim(' || asString(trank, true)||')';
    execute query into trank;
    insert into AAA (id_attr, a_val) values (20, trank);

-- get domain
    tbody :=  '/msg/passport/msg_created/as_having/community_domain_membership/text()';
 --   query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As domain';
 --   execute query INTO trank;
    select getXMLValue(tBody, value) into trank;
    query =  'select id from shu_domains where code = trim(' || asString(trank, true)||')';
    execute query into trank;
    insert into AAA (id_attr, a_val) values (230, trank);


-- priority

    tbody :=  '/msg/passport/msg_created/as_having/delivery_priority/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As prior';
--    execute query INTO deliveryPriority;
    select getXMLValue(tBody, value) into deliveryPriority;
    insert into AAA (id_attr, a_val) values (236, deliveryPriority);


-- name and text 
    tbody :=  '/msg/passport/msg_created/as_having/unformalized_document_specific_data_where/doc_name/text()';
--    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)';
--    execute query INTO tcode;
    select getXMLValue(tBody, value) into tcode;
    insert into AAA (id_attr, a_val) values (2, tcode);

    tbody :=  '/msg/body/unformalized_document_data/human_readable_text/text()';
--    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar)';
--    execute query INTO trank;
    select getXMLValue(tBody, value) into trank;
    insert into AAA (id_attr, a_val) values (237, trank);


-- get name post
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_name/text()';
--    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', ' || quote_literal(value) ||'::xml))[1]::varchar)';
--    execute query INTO tpostname;
    select getXMLValue(tBody, value) into tpostName;
    insert into AAA (id_attr, a_val) values (25, tpostname);


    insert into AAA (id_attr, a_val) values (232,'ufdoc');


    isFound = false;

    create temp table XXXX (tag_name varchar, the_name varchar, the_title varchar, the_type varchar, the_value varchar);
    
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
        --raise exception E'\n\n\n!!%!!\n\n\n', r.attr_value;
        --exit;
    end loop;

    drop table XXXX;

    if(isFound = false) then
        idCat = 168;
    else
/*        select ac.id_io_category into idCat
        from 
           (select ac.id_io_category, array_agg(ac.id_io_attribute) as arr
            from attrs_categories ac group by ac.id_io_category ) as ac
        where
            ac.arr = (select array_agg(id_attr) from AAA);
*/
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
            group by ac_ordered.id_io_category ) as ac
        where
            ac.arr = (select array_agg(aaa_ordered.id_attr) from (select id_attr from AAA order by 1) as aaa_ordered)
        order by 1
        limit 1;
       
        if(idCat isnull) then
            select cInsert('Новый входящий документ от 7т1 ' || tcode, NULL, NULL, 3, NULL, true, NULL, false, 1) into idCat;
            if(idCat isnull or idCat <= 0) then
                drop table AAA;
                raise exception 'Error! Cannot create category for new IO!';
                return false;
            end if;

            insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) select idCat, id_attr, NULL, false, false from AAA;
            --perform cSetGlobal(idCat);
        end if;
    end if;


    if (tcode is null) then
        tcode := 'Новый входящий документ';
    end if;

    ioName := tcode;

    select into idOrg id_organization from getlocalorg();

    select into idObject ioinsert (ioName, 
                                   idCat, 
                                   2/*jupiter*/, --author
                                   1, --id_state
                                   null::varchar, --table_name
                                   null::varchar, --desc
                                   NULL::varchar, --info
                                   1, --id_maclabel
                                   NULL::varchar, --unique_id
                                   1, --id_sync_type
                                   idOrg, --owner
                                   false, --is_global
                                   null::int4, --id_search_template
                                   null::varchar, --ref_table
                                   null::int8, --fill
                                   null::int8, --text_color
                                   1, --id_type
                                   NULL::VARCHAR,--r_icon
                                   null::uuid);--uuid_t
    if (idObject is null or idObject <= 0) then
        drop table AAA;
        return false;
    end if;
    
    for r in
        select id_attr, a_val from AAA
    loop
        select into idAttrRes ioInsertAttr (idObject, r.id_attr, quote_literal (r.a_val)::varchar, current_timestamp::timestamp without time zone, null::timestamp without time zone, null::int4, null::int4, null::varchar);
        if (idAttrRes is null or idAttrRes < 0) then
            drop table AAA;
            return null;
        end if;
    end loop;
 
    drop table AAA;

--
--Здесь необходимо вставить код, создающий прикрепленные файлы при их наличии
--

    select writeIOFiles (idObject, value) into idUrl;
/*    tbody := '/msg/passport/msg_created/as_having/attached_files_set/attached_file[@number=0]/text()';
    query := 'SELECT (xpath('|| quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml))[1] As attachments_count';
    execute query into attCnt;
    raise warning 'number of attachments is %', attCnt;
    if (attCnt is not null and attCnt > 0) then
        for i in 1..attCnt
        loop
            tbody := '/msg/body/attached_files_set where_files_amnt="' || attCnt || '"/attached_file[@number=' || i || ']/![CDATA[/text()';
            query := 'SELECT (xpath('|| quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml))[' || i || '] As attachments_body';
            execute query into attachments;
            fData_enc := attachments;
            select into fData decode (fData_enc, 'base64');
            select into idUrl rinserturl ('file_' || idObject || '_' || i, 'not assigned', 9, NULL::varchar);
            if (idUrl is not null and idUrl>0) then
                select into ioUrl ioinserturl (idObject, idUrl, 'file_' || idObject || '_' || i);
                if (ioUrl is not null and ioUrl > 0) then
                    select rWriteFile (idUrl, fData, 0);
                end if;
            end if;
        end loop;


    end if;
*/
--
--Далее идет код, добавляющий запись в журнал сообщений
--


    tbody :=  '/msg/passport/msg_created/to_be_delivered_to_addressees_where/addressee[@number=1]/has_uri/text()';
    select getXMLValue(tBody, value) into addresseeURI;
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As addressee';
--    execute query INTO addresseeURI;
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(addresseeURI) ||') and sd.id = sdp.id_shu_dls;';
    if(query isnull) then
        idDlReceiver = 4;
    else
        execute query into idDlReceiver;
        if(idDlReceiver isnull) then
            idDlReceiver = 4;  --admin
        end if;
    end if;
    

    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||',  '||quote_literal(value) ||'::xml))[1] As uri_post';
--    execute query INTO tpost;
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
--    query := 'SELECT trim((xpath(' || quote_literal(tbody) || ', ' || quote_literal(value) || '::xml))[1]::varchar)';
--    if(query isnull) then
--        msgBody = '';
--    end if;
--    execute query INTO msgBody;
    select getXMLValue(tBody, value) into msgBody;


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
                                id_urgency_level) 
    values
                                    
                               (idMessage, 
                                idDlSender, 
                                idDlReceiver,
                                idObject, 
                                current_timestamp, 
                                NULL, 
                                NULL,
                                NULL,
                                msgBody,
                                1);

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
 
        --query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar) As date';
        --execute query INTO receiptType;
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
        
        perform jms_schema.add_out_mes (muid || '-receipt', 0, 0, true, xml2Text);
    end loop;    

    return isMakeIO;

end
$BODY$
LANGUAGE plpgsql;
