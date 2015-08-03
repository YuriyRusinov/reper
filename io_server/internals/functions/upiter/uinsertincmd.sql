create or replace function uInsertInCmd(int4,
                                        int4,
                                        varchar,
                                        int4,
                                        varchar,
                                        int4,
                                        varchar,
                                        int4,
                                        varchar,
                                        timestamp,
                                        timestamp,
                                        int4[2],
                                        varchar,
                                        varchar,
                                        varchar,
                                        int4,
                                        int4) returns int4 as
$BODY$
declare
    extraId alias for $1;
    dl_from_id alias for $2; --����������� (��)
    dl_from_name alias for $3; --����������� (���) NULL
    dl_executor_id alias for $4; --����������� (��)
    dl_executor_name alias for $5; --����������� (���) NULL
    dl_to_id alias for $6; --���������� (��)
    dl_to_name alias for $7; --NULL
    id_category alias for $8;--���������, �� ������� ����������� �����
    category_code alias for $9; --NULL
    insertTime alias for $10;--current_timestamp
    execDatetime alias for $11; --
    execPeriod alias for $12;
    messageBody alias for $13;
    uniqueID alias for $14;
    outputNumber alias for $15;
    idUrgencyLevel alias for $16;
    idObject alias for $17;

    tmp int4;
    idCommand int4;
begin

    --���� � �������� unique_id ��� ���� ������������ � ������� � ��� ��������� = 1, 
    --�� ��� ��������, ��� ���� ��������� ����������� ������� (����� ������� ��� ����������� ������ ��������� �������� �� � 2, � � 1)
    --� ���� ������ ���� ������ �������� �� ��������� �� 2 � ������ ���� ��������� � �������
    select id into idCommand  from command_journal where unique_id = uniqueID and id_jr_state = 1;
    if(idCommand is not null) then
        update command_journal set 
            id_jr_state = 2, 
            insert_time = current_timestamp, 
            receive_datetime = current_timestamp, 
            extra_id = extraId
        where id = idCommand;
        
        return idCommand;
    end if;

    --������-�� ������ ��� �������� ������ ������ ��������.
    --�� �������, ��� ������� ������ ��������� ���� �� �����
    --������� ���� ��� ���� �� ��������� (�.�. � ������ UID), 
    --�� ��������, ��� ��������� ��� ����� ������ � ������ �� ������
    select id into idCommand  from command_journal where unique_id = uniqueID;
    if(idCommand is not null) then
        return idCommand;
    end if;

    select id into tmp from "position" where id = dl_from_id and name = dl_from_name;
    if(tmp isnull) then
        raise warning 'DL_FROM is incorrect! id=%, name=%', dl_from_id, dl_from_name;
        return -11;
    end if;

    select id into tmp from "position" where id = dl_executor_id and name = dl_executor_name;
    if(tmp isnull) then
        raise warning 'DL_EXECUTOR is incorrect! id=%, name=%', dl_executor_id, dl_executor_name;
        return -12;
    end if;

    select id into tmp from "position" where id = dl_to_id and name = dl_to_name;
    if(tmp isnull) then
        raise warning 'DL_TO is incorrect! id=%, name=%', dl_to_id, dl_to_name;
        return -13;
    end if;

    select id into tmp from io_categories where id = id_category;
    if(tmp isnull) then
        raise warning 'CATEGORY is incorrect! id=%, code=%', id_category, category_code;
        return -14;
    end if;

    select getNextSeq('command_journal', 'id') into tmp;
    insert into command_journal (id, 
                                 id_dl_from, 
                                 id_dl_executor, 
                                 id_dl_to, 
                                 id_io_category, 
                                 id_jr_state, 
                                 insert_time, 
                                 is_automated, 
                                 exec_datetime,
                                 exec_period,
                                 message_body,
                                 is_archived,
                                 extra_id,
                                 unique_id,
                                 output_number,
                                 id_urgency_level,
                                 id_io_object_att)
                        values
                                (tmp,
                                 dl_from_id,
                                 dl_executor_id,
                                 dl_to_id,
                                 id_category,
                                 1,
                                 insertTime,
                                 true,
                                 execDatetime,
                                 execPeriod,
                                 messageBody,
                                 false,
                                 extraId,
                                 uniqueID,
                                 outputNumber,
                                 idUrgencyLevel,
                                 idObject
                                );

    if(FOUND = FALSE) then
        raise warning 'Command does not inserted in command_journal!';
        return -15;
    end if;
    
    return tmp;

end
$BODY$
language 'plpgsql';
