CREATE OR REPLACE FUNCTION createrecpassport(integer, integer, integer, integer, integer[], integer)
  RETURNS xml AS
$BODY$
declare
    idObject alias for $1;
    idRec alias for $2;
    idMsg alias for $3;
    idDlSender alias for $4;
    idAddrList alias for $5;
    msgKind alias for $6;
    
    regNumber varchar;
    --msgKind int4; --1 - ufdoc, 2 - fdoc, 3 - cmd
    msgPriority int4; --1 - LOWEST 2 - INTERMEDIATE 3 - HIGHEST

    query varchar;
    i int4;
    rec record;
    rec_url record;
    fdata bytea;
    url varchar;

    result xml;
    regNum varchar;
    idPos int4;
    idShuPos int4;
    idUser int4;
    cnt int4;
    xml_str varchar;
    xml_val_str varchar;
    tableName varchar;
    recName varchar;
--    docRef varchar;
    wmType int4;

    fileSize int8;
    limitFileSize int8;

begin

    limitFileSize := 2048;
    limitFileSize := limitFileSize*1024;
    limitFileSize := limitFileSize*1024;

    idPos = idDlSender;
    if(idPos isnull) then
        select into idPos getcurrentdl();

        if (idPos is null or idPos < 0) then
            raise warning 'Invalid position';
            return null;
        end if;
    end if;


    query := 'select shu.id from shu_dls as shu inner join shu_dls_position p on (shu.id = p.id_shu_dls and p.id_position=' || idPos || ')';
    execute query into idShuPos;
    --raise warning 'idShuPos is %', idShuPos;
    if (idShuPos is null or idShuPos < 0) then
        raise warning 'Invalid sender';
        return null;
    end if;

    if (idRec is not null and idObject is not null) then
        --
        -- Reading table name from io_objects
        --
        select table_name into tableName from tbl_io_objects io where io.id=idObject;
        if (tableName is null) then
            raise warning 'Invalid reference';
            return null;
        end if;
        --
        -- If attribute name presents in category then I assign its value into record using very 
        -- complex query
        --
        for rec in 
            select a.id, a.code from attrs_categories ac inner join attributes a on (ac.id_io_attribute=a.id) inner join io_categories c on (ac.id_io_category=c.id_child) inner join tbl_io_objects io on (io.id_io_category=c.id and io.id=idObject and a.code=E'name')
        loop
            if (rec.code is not null) then
                query := E'select name from ' || tableName || E' where id=' || idRec;
                execute query into recName;
            else
                recName := E'';
            end if;
        end loop;
    else
        return null;
    end if;

    if(idMsg is not null) then
        for rec in select unique_id, id_urgency_level from message_journal where id = idMsg
        loop
            --if(rec.id_)
            regNumber = rec.unique_id;
            msgPriority = rec.id_urgency_level;
        end loop;
    end if;
    

    xml_str := E'\t<passport>\n';
    xml_str := xml_str || E'\t\t<msg_created>\n';

    xml_str := xml_str || E'\t\t\t<by_automated_control_system>\n';
    xml_str := xml_str || E'\t\t\t\t<with_uri>';

    query = 'select a.uri from shu_acs a inner join shu_dls p on (p.id_acs=a.id and p.id = ' || idShuPos || ');';
    execute query into xml_val_str;

    if (xml_val_str is null) then
        raise warning 'Invalid ACS URL';
        return null;
    end if;

    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</with_uri>\n';

    xml_str := xml_str || E'\t\t\t\t<with_working_mode>';

    select wm.id_mode_type into wmType from work_mode wm, organization o where o.id = (select id_organization from getMyOrganization()) and o.id_curr_mode = wm.id;
    if(wmType isnull or wmType <> 2) then
        wmType = 0;
    else
        wmType = 1;
    end if;

    xml_str := xml_str || wmType;
    xml_str := xml_str || E'</with_working_mode>\n';
    xml_str := xml_str || E'\t\t\t\t<on_address> <![CDATA[';

    /*select into xml_val_str a.uri from shu_acs a inner join shu_dls p on (p.id_org=a.id and p.id=idShuPos);
    raise warning 'arm uri is %', xml_val_str;
    if (xml_val_str is null) then
        raise warning 'Invalid organization URL';
        return null;
    end if;
    xml_str := xml_str || xml_val_str;*/

    xml_str := xml_str || E']]> </on_address>\n';

    xml_str := xml_str || E'\t\t\t</by_automated_control_system>\n';

    xml_str := xml_str || E'\t\t\t<by_organization_unit>\n';
    xml_str := xml_str || E'\t\t\t\t<with_uri>';

    select into xml_val_str o.uri from shu_orgs o inner join shu_dls p on (p.id_org=o.id and p.id=idShuPos);

    if (xml_val_str is null) then
        raise warning 'Invalid org URL';
        return null;
    end if;

    xml_str := xml_str || xml_val_str;

    xml_str := xml_str || E'</with_uri>\n';
    xml_str := xml_str || E'\t\t\t</by_organization_unit>\n';
    xml_str := xml_str || E'\t\t\t<by_post_unit>\n';
    xml_str := xml_str || E'\t\t\t\t<with_uri>';

    select into xml_val_str p.uri from shu_dls p where p.id=idShuPos;
    --raise warning 'pos uri is %', xml_val_str;
    if (xml_val_str is null) then
        raise warning 'Invalid pos URL';
        return null;
    end if;

    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</with_uri>\n';
    xml_str := xml_str || E'\t\t\t\t<with_post_id>';
    select into xml_val_str pos.code from shu_positions pos inner join shu_dls p on (p.id_pos=pos.id and p.id=idShuPos);
    --raise warning 'pos code is %', xml_val_str;
    if (xml_val_str is null) then
        raise warning 'Invalid pos code';
        return null;
    end if;
    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</with_post_id>\n';

    xml_str := xml_str || E'\t\t\t\t<with_rank_id>';
    select into idUser getcurrentuser();
    if (idUser is null or idUser <0) then
        raise warning 'Invalid user';
        return null;
    end if;
    select into xml_val_str r.code from ranks r inner join users u on (u.id_rank=r.id and u.id=idUser);
    if (xml_val_str is not null) then
        xml_str := xml_str || xml_val_str;
    end if;
    xml_str := xml_str || E'</with_rank_id>\n';

    xml_str := xml_str || E'\t\t\t\t<with_name><![CDATA[';
    select into xml_val_str u.fio from users u where u.id=idUser;
    if (xml_val_str is not null) then
        xml_str := xml_str || xml_val_str;
    end if;
    xml_str := xml_str || E']]></with_name>\n';

    xml_str := xml_str || E'\t\t\t</by_post_unit>\n';

    xml_str := xml_str || E'\t\t\t<as_having>\n';
    xml_str := xml_str || E'\t\t\t\t<kind>ufrec</kind>\n';

    xml_str := xml_str || E'\t\t\t\t<creation_date>';
    select into xml_val_str to_char(now()::date, 'DD.MM.YYYY');
    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</creation_date>\n';
    xml_str := xml_str || E'\t\t\t\t<creation_time>';
    select into xml_val_str to_char(now()::time, 'HH12:MI:SS');
    xml_str := xml_str || xml_val_str;
    xml_str := xml_str || E'</creation_time>\n';

    xml_str := xml_str || E'\t\t\t\t<sending_acs_registration_number>';
    if(regNumber is not null) then
        xml_str = xml_str || regNumber;
    else
        select unique_id into regNum from io_objects where id = idRec;
        if(regNum is not null) then
            xml_str = xml_str || regNum;
        end if;
    end if;
    xml_str := xml_str || E'</sending_acs_registration_number>\n';

    xml_str := xml_str || E'\t\t\t\t<redirecting_acs_registration_number>';
    xml_str := xml_str || E'</redirecting_acs_registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t<mac_level>';
    xml_str := xml_str || 0;
    xml_str := xml_str || E'</mac_level>\n';

    xml_str := xml_str || E'\t\t\t\t<delivery_priority>';
    if(msgPriority isnull or msgPriority = 1) then
        xml_str := xml_str || 'LOWEST';
    elsif(msgPriority = 2) then 
        xml_str := xml_str || 'INTERMEDIATE';
    else 
        xml_str := xml_str || 'HIGHEST';
    end if;
    xml_str := xml_str || E'</delivery_priority>\n';

    xml_str := xml_str || E'\t\t\t\t<community_domain_membership>';
    xml_str := xml_str || 'composite';
    xml_str := xml_str || E'</community_domain_membership>\n';

    xml_str := xml_str || E'\t\t\t\t<command_specific_data_where>\n';
    xml_str := xml_str || E'\t\t\t\t\t<cmd_code><![CDATA[ ]]></cmd_code>\n';
    xml_str := xml_str || E'\t\t\t\t</command_specific_data_where>\n';

    xml_str := xml_str || E'\t\t\t\t<formalized_document_specific_data_where>\n';
    xml_str := xml_str || E'\t\t\t\t\t<doc_kind>';
    if(msgKind = 2) then
        xml_str := xml_str || E'info.up.1.1';
    end if;
    xml_str := xml_str || E'</doc_kind>\n';
     
    xml_str := xml_str || E'\t\t\t\t\t<doc_name><![CDATA[';
    if(msgKind = 2 and recName is not null) then
        xml_str := xml_str || recName;
    end if;
    xml_str := xml_str || E']]></doc_name>\n';
/*
    xml_str := xml_str || E'\t\t\t\t\t<doc_referat><![CDATA[';
    if(msgKind = 2 and docRef is not null) then
        xml_str := xml_str || docRef;
    end if;
    xml_str := xml_str || E']]></doc_referat>\n';
*/
    xml_str := xml_str || E'\t\t\t\t</formalized_document_specific_data_where>\n';

    xml_str := xml_str || E'\t\t\t\t<unformalized_document_specific_data_where>\n';
    xml_str := xml_str || E'\t\t\t\t\t<doc_name><![CDATA[';
    if(msgKind = 1 and recName is not null) then
        xml_str := xml_str || recName;
    end if;
    xml_str := xml_str || E']]></doc_name>\n';

    xml_str := xml_str || E'\t\t\t\t</unformalized_document_specific_data_where>\n';

    select count(*) into cnt from urls_objects uo where uo.id_io_object=idRec;
    xml_str := xml_str || E'\t\t\t\t<attached_files_set with_files_amnt="' || cnt || E'">\n';
    if (cnt > 0) then
        i := 1;
        for rec_url in
            select ur.id_url, ur.name from urls_records ur where ur.id_record=idRec
        loop
            xml_str := xml_str || E'\t\t\t\t\t<where_attached_file number="' || i || E'">\n';
            xml_str := xml_str || E'\t\t\t\t\t\t<has_name> <![CDATA[' || rec_url.name || E']]> </has_name>\n';
            xml_str := xml_str || E'\t\t\t\t\t\t<has_content_type>' || getUrlMimeType (rec_url.id_url) || E'</has_content_type>\n';
            select into fileSize rGetFileSize(rec_url.id_url);
            if (fileSize > limitFileSize) then
                raise warning 'Too big file, size is %', fileSize;
                return null;
            end if;

            xml_str := xml_str || E'\t\t\t\t\t\t<has_size>' || fileSize || E'</has_size>\n';
            xml_str := xml_str || E'\t\t\t\t\t</where_attached_file>\n';
            i := i+1;
        end loop;
    end if;
    xml_str := xml_str || E'\t\t\t\t</attached_files_set>\n';

    xml_str := xml_str || E'\t\t\t\t<response_msg_specific_data><![CDATA[ ]]></response_msg_specific_data>\n';

    xml_str := xml_str || E'\t\t\t\t<parent_msg_which>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_kind></has_kind>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_creation_date></has_creation_date>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_creation_time></has_creation_time>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_sending_acs_registration_number></has_sending_acs_registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_redirecting_acs_registration_number></has_redirecting_acs_registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_receiving_acs_registration_number></has_receiving_acs_registration_number>\n';
    xml_str := xml_str || E'\t\t\t\t\t<has_response_msg_specific_data></has_response_msg_specific_data>\n';
    xml_str := xml_str || E'\t\t\t\t</parent_msg_which>\n';


    xml_str := xml_str || E'\t\t\t</as_having>\n';
    cnt := 0;


    -- ,   .
    --    (    22)
    if (idAddrList is not null) then

        query := 'select count(*) from shu_dls addr where addr.id in(';
        for i in 1..array_upper (idAddrList, 1) loop
            query := query || idAddrList[i];
            if (i < array_upper (idAddrList, 1)) then
                query := query || ',';
            end if;
        end loop;
        query := query || ')';

        execute query into cnt;

        if (cnt is not null and cnt > 0) then

            xml_str := xml_str || E'\t\t\t<to_be_delivered_to_addressees_where addresees_amount="' || cnt || E'">\n';
            query := 'select addr.id, addr.uri from shu_dls addr where addr.id in(';

            for i in 1..array_upper (idAddrList, 1) loop
                query := query || idAddrList[i];
                if (i < array_upper (idAddrList, 1)) then
                    query := query || ',';
                end if;
            end loop;
            query := query || ')';

            i := 1;
            for rec in 
                execute query
            loop
                xml_str := xml_str || E'\t\t\t\t<addressee number="' || i || E'">\n';
                xml_str := xml_str || E'\t\t\t\t\t<has_type> post_unit </has_type>\n';
                xml_str := xml_str || E'\t\t\t\t\t<has_uri>' || rec.uri || E'</has_uri>\n';
                xml_str := xml_str || E'\t\t\t\t</addressee>\n';
                i := i+1;
            end loop;
            xml_str := xml_str || E'\t\t\t</to_be_delivered_to_addressees_where>\n';
        end if;
    end if;

    xml_str := xml_str || E'\t\t\t<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="1">\n';
    xml_str := xml_str || E'\t\t\t\t<receipt number="1">\n';
    xml_str := xml_str || E'\t\t\t\t\t<with_type> DR.3 </with_type>\n';
    xml_str := xml_str || E'\t\t\t\t\t<for_each_addressee> true </for_each_addressee>\n';
    xml_str := xml_str || E'\t\t\t\t\t<with_specific_data> <![CDATA[ ]]> </with_specific_data>\n';
    xml_str := xml_str || E'\t\t\t\t</receipt>\n';
    xml_str := xml_str || E'\t\t\t</to_be_confirmed_about_delivering_by_receipts_where>\n';

    xml_str := xml_str || E'\t\t</msg_created>\n';
    xml_str := xml_str || E'\t</passport>\n';
    result := xmlparse (document (xml_str));
    return result;
end
$BODY$
LANGUAGE plpgsql;
