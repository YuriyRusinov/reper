
CREATE OR REPLACE FUNCTION xml_out_message()
  RETURNS trigger AS
$BODY$
declare
    muid varchar;
    idt int4;
    val xml;

    idShuDl int4;
    ids int4[];
    xml2Text text;
begin
    muid := new.unique_id;

    if(isLocalDl(new.id_dl_receiver) = TRUE) then
        return new;
    end if;

    select id_shu_dls into idShuDl from shu_dls_position where id_position = new.id_dl_receiver;
    if(idShuDl isnull) then
        return new;
    end if;
    

    ids[1] = idShuDl;

    select into val ioToXML (new.id, new.id_io_object, ids);
    raise warning 'ioToXML parameters are id=% % % %', new.id, new.id_io_object, ids[1], val;
    if(val isnull) then
        raise exception E'Error! This message should be sended to legacy system\'s position. But XML has NULL value. Nothing to send!';
        return NULL;
    end if;

    xml2Text = xmlText(val);
    if(xml2Text isnull or trim(xml2Text) = '') then
        raise exception E'Error! This message should be sended to legacy system\'s position. But XML2Text has NULL value. Nothing to send!';
        return NULL;
    end if;

    

    select jms_schema.add_out_mes (muid, 0, 0, false, xml2Text) into idt;

    return new;
end
$BODY$
language 'plpgsql' security definer;

select f_safe_drop_trigger('zz_trg_out_message', 'message_journal');

CREATE TRIGGER zz_trg_out_message
  AFTER INSERT
  ON message_journal
  FOR EACH ROW
  EXECUTE PROCEDURE xml_out_message();
--select f_create_trigger('zz_trgzioinsertxmlafter', 'after', 'insert', 'io_objects', 'xmlinsertiorec(););


