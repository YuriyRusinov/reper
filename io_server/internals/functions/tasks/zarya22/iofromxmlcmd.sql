CREATE OR REPLACE FUNCTION iofromxmlcmdsimple(character varying)
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

    deliveryPriority varchar;
    msgCreationDate varchar;
    msgCreationTime varchar;
    msgSenderUID varchar;
    msgRedirectingNumber varchar;


    isFound bool;
    idCat int4;
    idOrg int4;
    idObject int4;
    idAttrType int4;
    ioName varchar;
    uid varchar;
    idAttrRes int4;
    idAttr int4;
    val varchar;
    isMakeIO boolean;
    r RECORD;

    --for cmd journal
    idDlTo int4;
    idDlExec int4;
    idCategory int4;
    execDateTime timestamp;
    idCommand int4;
    addresseeURI varchar;

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
    
    create temp table XXX (id_attr int4, a_val varchar);

    -- get cmd code
    tbody := '/msg/passport/msg_created/as_having/command_specific_data_where/cmd_code/text()';
    select getXMLValue(tBody, value) into tcode;
    insert into XXX (id_attr, a_val) values (238, tcode);
   
    -- get date creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_date/text()';
    select getXMLValue(tBody, value) into msgCreationDate;
    insert into XXX (id_attr, a_val) values (233, msgCreationDate);

    -- get time creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_time/text()';
    select getXMLValue(tBody, value) into msgCreationTime;
    insert into XXX (id_attr, a_val) values (234, msgCreationTime);

    -- get sending_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/sending_acs_registration_number/text()';
    select getXMLValue(tBody, value) into msgSenderUID;

    -- get redirecting_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/redirecting_acs_registration_number/text()';
    select getXMLValue(tBody, value) into msgRedirectingNumber;

    -- get mac level message
    tbody :=  '/msg/passport/msg_created/as_having/mac_level/text()';
    select getXMLValue(tBody, value) into tmac;
    insert into XXX (id_attr, a_val) values (235, tmac);


    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_working_mode/text()';
    select getXMLValue(tBody, value) into torg;
    insert into XXX (id_attr, a_val) values (231, torg);
    

    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_uri/text()';
    select getXMLValue(tBody, value) into torg;
    query =  'select id from shu_acs where uri = trim(' || quote_literal(torg) || ')';
    if(query is not null) then
        execute query into torg;
        insert into XXX (id_attr, a_val) values (225, torg);
    --else 
        --insert into XXX (id_attr, a_val) values (225, ' ');
    end if;

    -- get organization uri
    tbody :=  '/msg/passport/msg_created/by_organization_unit/with_uri/text()';
    select getXMLValue(tBody, value) into torg;
    query =  'select id from shu_orgs where uri = trim(' || quote_literal(torg) || ')';
    if(query is not null) then
        execute query into torg;
        insert into XXX (id_attr, a_val) values (226, torg);
    end if;

-- get post uri
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
    select getXMLValue(tBody, value) into tpost;
    query =  'select id from shu_dls where uri = trim(' || quote_literal(tpost) || ')';
    if(query is not null) then
        execute query into tpost;
        insert into XXX (id_attr, a_val) values (228, tpost);
    end if;


-- get post id

--    tbody :=  '/msg/passport/msg_created/by_post_unit/with_post_id/text()';
--    query := 'SELECT (xpath('||quote_literal(tbody) ||', cast '||quote_literal(value) ||' as xml))[1] As post_id';

--   execute query INTO tpost_id;	


-- get rank id
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_rank_id/text()';
    select getXMLValue(tBody, value) into trank;
    query =  'select id from ranks where code = trim(' || quote_literal(trank) || ')';
    if(query is not null) then
        execute query into trank;
        insert into XXX (id_attr, a_val) values (20, trank);
    end if;


-- get name post
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_name/text()';
    select getXMLValue(tBody, value) into tpostname;
    insert into XXX (id_attr, a_val) values (25, tpostname);

-- get command body

    tbody := '/msg/body/command_data/human_readable_text/text()'; 
    select getXMLValue(tBody, value) into tbody;
    insert into XXX (id_attr, a_val) values (239, tbody);

--     type message

    insert into XXX (id_attr, a_val) values (232,'cmd');

    -- get domain
    tbody :=  '/msg/passport/msg_created/as_having/community_domain_membership/text()';

    select getXMLValue(tBody, value) into trank;
    query =  'select id from shu_domains where code = trim(' || quote_literal(trank) || ')';
    if(query is not null) then
        execute query into trank;
        insert into XXX (id_attr, a_val) values (230, trank);
    end if;


-- priority

    tbody :=  '/msg/passport/msg_created/as_having/delivery_priority/text()';
    select getXMLValue(tBody, value) into deliveryPriority;
    insert into XXX (id_attr, a_val) values (236, deliveryPriority);
    
 
  -- make category
  --���� ������� �������, �� ��������� ��������� �� ����
  -- idCat = 169
    isFound = false;

    create temp table XXXX (tag_name varchar, the_name varchar, the_title varchar, the_type varchar, the_value varchar);

    for r in select * from getXMLParams(value)
    loop
        isFound = true;

        idAttrType = getXMLAttrType(r.attr_type);
        select aInsert(idAttrType, r.attr_code, r.attr_name, r.attr_title, NULL, NULL, NULL, NULL) into idAttr;
        if(idAttr isnull or idAttr <= 0) then
            drop table XXX;
            drop table XXXX;
            raise exception 'Cannot create attribute';
            return false;
        end if;
        insert into XXX (id_attr, a_val) values (idAttr, r.attr_value);
    end loop;

    drop table XXXX;

    if(isFound = false) then
        idCat = 169;
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
            group by ac_ordered.id_io_category ) as ac
        where
            ac.arr = (select array_agg(aaa_ordered.id_attr) from (select id_attr from XXX order by 1) as aaa_ordered)
        order by 1
        limit 1;
       
        if(idCat isnull) then
            select cInsert('������� �� 7�1 � ����� ' || tcode, NULL, NULL, 3, NULL, true, NULL, false, 1) into idCat;
            if(idCat isnull or idCat <= 0) then
                raise exception 'Error! Cannot create category for new IO!';
                return false;
            end if;

            insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) select idCat, id_attr, NULL, false, false from XXX;
            --perform cSetGlobal(idCat);
        end if;
    end if;


    ioName := '������� �� 7�1 � ����� ' || tcode;
    if(ioName isnull) then
        ioName = '����� ������� �� 7�1 � ����������� �����';
    end if;

    select into idOrg id_organization from getlocalorg();

    select into idObject ioinsert (ioName, 
                                   idCat, 
                                   2/*jupiter*/, --author
                                   1, --id_io_state
                                   null::varchar, --table_name
                                   null::varchar, --desc
                                   NULL::varchar, --info
                                   1, --id_maclabel
                                   NULL::varchar, --unique_id
                                   1, --id_sync_type
                                   idOrg, --id_owner_org
                                   false, --is_global
                                   null::int4, --id_search_template
                                   null::varchar, --ref_table_name
                                   null::int8, --record_fill_color
                                   null::int8, --record_text_color
                                   1, --id_type
                                   NULL::VARCHAR,--r_icon
                                   NULL::uuid);--uuid_t
    --raise warning 'Document id is %', idObject;
    if (idObject is null or idObject <= 0) then
        return false;
    end if;
    

    for r in
        select id_attr, a_val from XXX
    loop
        select into idAttrRes ioInsertAttr (idObject, r.id_attr, quote_literal (r.a_val)::varchar, current_timestamp::timestamp without time zone, null::timestamp without time zone, null::int4, null::int4, null::varchar);
        if (idAttrRes is null or idAttrRes <0) then
            return null;
        end if;
    end loop;
 
    drop table XXX;

    tbody :=  '/msg/passport/msg_created/to_be_delivered_to_addressees_where/addressee[@number=1]/has_uri/text()';
    select getXMLValue(tBody, value) into addresseeURI;
    
    --���������� ����������� �������
    --���� � �������� �� ��� ����������� ��, �� ���������� �������, ��� ������� ������ ��������� ������������� �������� ��
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(addresseeURI) ||') and sd.id = sdp.id_shu_dls;';
    if(query is not null) then
        execute query into idDlExec;
        if(idDlExec isnull) then
            idDlExec = 4;  --admin
        end if;
    else
        idDlExec = 4;
    end if;
    
    --���������� ����������� ������� (���������� ���������� + ���������� ����������� ����������)
    --���� � �������� �� ��� ����������� ��, �� ���������� �������, ��� ������� ����� ������������� �������� ��
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
    select getXMLValue(tBody, value) into tpost;
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(tpost) ||') and sd.id = sdp.id_shu_dls;';
    if(query is not null) then
        execute query into idDlTo;
        if(idDlTo isnull) then
            idDlTo = 4;  --admin
        end if;
    else
        idDlTo = 4;
    end if;


    idCategory = getZaryaCommandCategory();
    execDateTime = getZaryaExecDatetime();

    if(idCategory isnull or idCategory <= 0) then
        idCategory = 1;
    end if;

    if(execDateTime isnull) then
        execDatetime = current_timestamp + interval '2 hours';
    end if;

    if(idDlTo isnull or idDlExec isnull or idCategory isnull) then
        raise warning 'Cannot create command in command journal!';
        if(getCurrentUser() <> 2) then -- jupiter
            insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
            values(idObject, -10, true, false, false, false, false);
        end if;
        return false;
    end if;

    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
    values(idObject, idDlTo, true, true, true, true, true);

    if(idDlExec <> idDlTo) then
        insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
        values(idObject, idDlExec, true, true, true, true, true);
    end if;


    select getNextSeq('command_journal', 'id') into idCommand;

    raise warning E'\n!!!! DlFrom = %, DlTo = %. DlExec = %\n', idDlTo, idDlTo, idDlExec;
    
    insert into command_journal(id, 
                                id_dl_from, 
                                id_dl_to, 
                                id_dl_executor,
                                id_io_category,
                                id_jr_state,
                                insert_time,
                                exec_datetime,
                                exec_period,
                                message_body,
                                id_urgency_level,
                                id_io_object_att,
                                is_automated,
                                is_archived,
                                output_number)
    values
                                    
                               (idCommand, 
                                idDlTo, 
                                idDlTo,
                                idDlExec,
                                idCategory,
                                1,--������������
                                current_timestamp,
                                execDateTime,
                                NULL, 
                                ioName,
                                1,
                                idObject,
                                true,
                                false,
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

        select receiptToXML(receiptType, deliveryPriority, 'cmd', msgCreationDate, msgCreationTime, msgSenderUID, addresseeURI, msgRedirectingNumber) into receiptXML;
        if(receiptXML isnull) then
            continue;
        end if;

        xml2Text = xmlText(receiptXML);

        select unique_id into muid from command_journal where id = idCommand;
        if(muid isnull) then
            continue;
        end if;
        
        --ksa perform jms_schema.add_out_mes (muid || '-receipt', 0, 0, true, xml2Text);
    end loop;    
    

    return isMakeIO;

end
$BODY$
LANGUAGE plpgsql;
