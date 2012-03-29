create or replace function tsdInsertCheck() returns trigger as
$BODY$
declare
    res int4;

    query varchar;

    jrTableName varchar; --�������� ������� �������� ������������
    messBody varchar; --message body

    idDl int4;
    ok bool;
begin
    
    perform clearLastError();

    ok = false;

    if(isLocalDl(new.id_dl_to) = true and isLocalDl(new.id_dl_executor) = true and isLocalDl(new.id_dl_controller) = true ) then
        ok = true;
    elsif(isPublicDl(new.id_dl_to) = true and isPublicDl(new.id_dl_executor) = true and isPublicDl(new.id_dl_controller) = true ) then
        ok = true;
    end if;
        

    if(ok = false) then
        raise exception 'adressee, executor and controller must be all either public or local!';
        return NULL;
    end if;

    messBody := '�� ���������� ������ ' || new.id || ' ������ ������� ���������';

    jrTableName := 'command_journal';
        
    --select getCurrentDl() into idDl;
    
    query := 
       ' insert into ' || jrTableName ||
       ' (id_dl_from, id_dl_executor, id_dl_to, id_io_category, id_jr_state, 
          insert_time, is_automated, exec_datetime, exec_period, message_body, is_archived, id_tabel, last_exec_datetime) values ( ' ||
            new.id_dl_to || ', ' ||  --���� ����������� ������ (�������, ��� ��� � �������� command_journal ����� ������������)
            new.id_dl_executor || ', ' || --�����������
            new.id_dl_controller || ', ' ||  --����������
            new.id_io_category || ', ' ||
            '1' || ', ' ||
            'current_timestamp' || ', ' ||
            'true' || ', ' ||
            quote_literal(new.exec_datetime) || ', ' ||
            asString(new.exec_period, true) || ', ' ||
            quote_literal(messBody) || ', ' ||
            'false' || ', ' ||
            new.id || ', ' ||
            quote_literal(new.exec_datetime) || ')';

    execute query;
    
    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgtsdinsert', 'tsd');

create trigger trgTsdInsert
after insert or update
on tsd 
for each row 
execute procedure tsdInsertCheck();
