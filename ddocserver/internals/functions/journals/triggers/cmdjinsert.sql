create or replace function cmdJInsertCheck() returns trigger as
$BODY$
declare
    res int4;
    idDl int4;

    tableName varchar;
    localId int4;

    inputNumber varchar;
    outputNumber varchar;

    idUser int4;
    ok bool;
begin
    
    perform clearLastError();


        
    select getCurrentDl() into idDl;
    
    
    select checkForPublic(new.id_dl_from, new.id_dl_executor, new.id_dl_to) into ok;
    if(ok <> TRUE) then
        raise warning 'You cannot send command to remote public position, bacause of you are not on public position!';
        return NULL;
    end if;

    if(TG_OP = 'INSERT') then
        --raise notice 'NEW.ID_JR_STATE = %', new.id_jr_state;
        if(new.exec_period is not null) then
            new.last_exec_datetime = new.exec_datetime;
        end if;

        tableName = TG_RELNAME;
        localId = new.id;

        idUser = getCurrentUser();
        --учетные номера
        --входящий и исходящий
        --если распоряжение в журнал попало от имени обычного пользователя (не jupiter !)
        --то мы должны поставить ему исходящий номер, а входящий надо оставить пустым (но не NULL)
        --если от имени jupiter, то распоряжение является входящим из удаленной системы и мы ему задаем входящий. исходящий оставляем неизменным
        if(idUser = 2) then --jupiter
            new.input_number = generateUID(localId, tableName);
            if(new.output_number isnull) then
                new.output_number = '';
            end if;
            --new.output_number = -- без изменений
        else
            new.output_number = generateUID(localId, tableName);
            new.input_number = '';
        end if;

        if(new.input_number isnull or new.output_number isnull) then
            raise exception 'Cannot generate input_number and output_number for the record! TABLE = %, LOCALID = %', tableName, localId;
            return NULL;
        end if;
    
        if(new.id_urgency_level isnull) then
            new.id_urgency_level = 1;
        end if;


        if(isLocalDl(new.id_dl_executor) = TRUE) then
            new.receive_datetime = current_timestamp;
        else
            new.receive_datetime = NULL;
            new.accepted_datetime = NULL;
        end if;

        if(idUser = 2) then
            /*
            --jupiter (we insert the record that come from another organization)
            --insert into cmd_confirmations (id_cmd) values(new.id);--update receive_datetime on sender
            */
            if (isLocalDl(new.id_dl_executor) = TRUE) then
                new.id_jr_state = 2;
                --problem was set
            else
                new.id_jr_state = 1;--planned (for virtuals)
            end if;
            
            return new; --output_number generated in another organization
        end if;

        return new;
    end if;

    --TG_OP = ''UPDATE''

    if(new.input_number <> old.input_number) then
        raise exception 'ATTRIBUTE INPUT_NUMBER MUST BE UNCHANGED!';
        return NULL;
    end if;

    if(new.output_number <> old.output_number) then
        raise exception 'ATTRIBUTE OUTPUT_NUMBER MUST BE UNCHANGED!';
        return NULL;
    end if;

    if(old.id_jr_state <> new.id_jr_state and new.id_jr_state = 3) then --executing

--TODO 
--надо проверять, а надо ли добавлять запись в эту таблицу. Она используется при информационном обмене.
--поэтому если команда выдается локально или пришла из внешней унаследованной системы, то наверное сюда
--добавлять ничего не надо
        if(isLocalDl(new.id_dl_from) = false or isLocalDl(new.id_dl_to) = false) then
            insert into cmd_confirmations (id_cmd) values(new.id);
        end if;

    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcmdjinsert', 'command_journal');

create trigger trgCmdJInsert
before insert or update
on command_journal 
for each row 
execute procedure cmdJInsertCheck();

create or replace function cmdJInsertCheckAfter() returns trigger as
$BODY$
declare

    idUser int4;
begin
    
    idUser = getCurrentUser();

    if(TG_OP = 'INSERT') then

    
        if(idUser = 2 and isLocalDl(new.id_dl_executor) = TRUE) then --jupiter (we insert the record that come from another organization)
                                                                     --skip virtual orders
            
            insert into cmd_confirmations (id_cmd) values(new.id);--update receive_datetime on sender
        end if;
        
    end if;

    if(TG_OP = 'UPDATE') then
        if(idUser = 2 and isLocalDl(new.id_dl_executor) = FALSE) then --jupiter (we insert the record that come from another organization)
                                                                     --skip virtual orders
            if(old.id_jr_state = 7 and (new.id_jr_state = 1 or new.id_jr_state = 2)) then
                insert into cmd_confirmations (id_cmd) values(new.id);--update receive_datetime on sender
            end if;
        end if;
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgcmdjinsertafter', 'command_journal');

create trigger trgCmdJInsertAfter
after insert or update
on command_journal 
for each row 
execute procedure cmdJInsertCheckAfter();
