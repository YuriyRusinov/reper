create or replace function xWriteIOFiles (int4, varchar) returns int4 as
$BODY$
declare
    idObject alias for $1;
    value alias for $2;

    tbody varchar;
    query varchar;
    attCnt int4;
    i int4;

    attNumbers int4[];
    attPasspNumbers int4[];
    attPasspCnt int4;
    attachments varchar;
    fData bytea;
    fData_enc varchar;
    idUrl int4;
    ioUrl int4;
    idUrlType int4;
    mimetype varchar[];
    fd int4;
    ii int4;
    blocksize int4;
    blockdata bytea;
begin
    tbody := '/msg/passport/msg_created/as_having/attached_files_set/where_attached_file/@number';
    query := 'SELECT (xpath('|| quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml)) As attachments_pcount';
    execute query into attPasspNumbers;
    attPasspCnt := array_upper (attPasspNumbers, 1);
    tbody := '/msg/body/attached_files_set/attached_file/@number';
    --/msg/body/attached_files_set_where/files_amnt[@number=0]/text()';
    query := 'SELECT (xpath('|| quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml)) As attachments_count';
    --raise warning 'query is %', query;
    execute query into attNumbers;
    attCnt := array_upper (attNumbers, 1);
    raise warning 'number of attachments is %', attCnt;
    if (attPasspCnt != attCnt) then
        return NULL::int4;
    end if;
    tbody := '/msg/passport/msg_created/as_having/attached_files_set/where_attached_file/has_content_type/text()';
    query := 'SELECT (xpath('|| quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml)) As attachment_type';
    execute query into mimetype;
    if (attCnt is not null and attCnt > 0) then
        for i in 1..attCnt
        loop
            tbody := '/msg/body/attached_files_set/attached_file[@number=' || attNumbers[i] || ']/text()';
            select getXMLValue (tbody, value) into attachments;
            --raise warning 'attachment is %', attachments;
            raise warning 'mime type is %', mimetype[i];
            select into idUrlType getPassportUrlType (mimetype[i]);
            if (idUrlType is null) then
                idUrlType := 1;/*simple text*/
            end if;
            fData_enc := attachments;
            select into fData decode (fData_enc, 'base64');
            --raise warning 'decoded data are %', fData;
            select into idUrl rinserturl ('file_' || idObject || '_' || i, 'not assigned', idUrlType, NULL::varchar);
            if (idUrl is not null and idUrl>0) then
                select into ioUrl ioinserturl (idObject, idUrl, 'file_' || idObject || '_' || i);
                raise warning 'idUrl is % ioUrl is %', idUrl, ioUrl;
                if (ioUrl is not null and ioUrl > 0) then
                    ii := 0;
                    loop
                        blocksize := 102400;
                        blockdata := substr (fData, ii*blocksize, blocksize);
                        if (blockdata is null or octet_length (blockdata) = 0) then
                            exit;
                        end if;
                        if (ii = 0) then
                            select rWriteFile (idUrl, blockData, 0) into fd;
                        else
                            select rWriteFile (idUrl, blockData, 1) into fd;
                        end if;
                        ii := ii+1;
                    end loop;
                end if;
            end if;
        end loop;
        return 1;
    else
        return NULL::int4;
    end if;

    return 1;
end
$BODY$
language 'plpgsql';
