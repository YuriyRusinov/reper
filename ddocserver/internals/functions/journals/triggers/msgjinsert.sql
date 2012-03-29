--drop trigger trgmsgjinsert on message_journal;
create or replace function msgJInsertCheck() returns trigger as
$BODY$
declare
    res int4;
    idDl int4;
    isLocal boolean;

    tableName varchar;
    localId int4;

    inputNumber varchar;
    outputNumber varchar;

    idUser int4;
    ok bool;
begin
    
    perform clearLastError();


        
    select getCurrentDl() into idDl;
    

    select checkForPublic(new.id_dl_sender, new.id_dl_receiver) into ok;
    if(ok <> TRUE) then
        raise warning 'You cannot send message to remote public position, bacause of you are not on public position!';
        return NULL;
    end if;

    if(TG_OP = 'UPDATE') then

        if(new.input_number <> old.input_number) then
            raise exception 'ATTRIBUTE INPUT_NUMBER MUST BE UNCHANGED!';
            return NULL;
        end if;

        if(new.output_number <> old.output_number) then
            raise exception 'ATTRIBUTE OUTPUT_NUMBER MUST BE UNCHANGED!';
            return NULL;
        end if;

        if(new.read_datetime is not null and old.read_datetime isnull) then
            --≈я ’ ќЌ —¬ё–≈ Ё яЌЌјџ≈ћ’Џ ЏЅ Џ≈–яЏ  Ќ…ё ЁћЎЋ, ё Ќ–ќѕёЅ’–≈ Ё —ƒё ≈ћћЎЋ,
            --–Ќ÷ƒё ћ≈Ќј”Ќƒ’ЋЌ ЅЎя–ёЅ’–Ё ƒёћћЌ≈ яЌЌјџ≈ћ’≈ …ё… ћ≈Ќ–ќѕёЅ ≈ћћЌ≈,
            --ќЌя…Ќ Ё…— ћ≈Ќј”Ќƒ’ЋЌ Ќ–ќѕёЅ’–Ё Ќ–ќѕёЅ’–≈ „ …Ѕ’–ёћ∆’„ Ќ ќѕЌ¬–≈ћ’’.
            --яёЋЌ яЌЌјџ≈ћ’≈ ќѕ’ ў–ЌЋ ћ≈ Ќ–ќѕёЅ’–яЏ,
            --ќЌя…Ќ Ё…— “—ћ…∆’Џ ќЌ’я…ё яЌЌјџ≈ћ’» ћё Ќ–ќѕёЅ…— –ё…‘≈ ƒ≈ ё≈– ќЌƒЌјћ—„ ќѕЌЅ≈ѕ…—
            if(old.is_outed = true and isLocalDl(old.id_dl_receiver) = TRUE and isLocalDl(old.id_dl_sender) = FALSE) then
                new.is_outed = false;
            end if;
        end if;

        return new;
    end if;

    --TG_OP = INSERT
    if(new.id_urgency_level isnull) then
        new.id_urgency_level = 1;
    end if;

    if(new.sent_datetime isnull) then
        new.sent_datetime = current_timestamp;
    end if;

    idUser = getCurrentUser();

    if(idUser = 2) then --jupiter
        new.read_datetime = NULL;
        new.receive_datetime = current_timestamp;
        new.is_outed = false;
    else
        select isLocalDl(new.id_dl_receiver) into isLocal;
        if(isLocal = true) then
            new.is_outed = true;
            new.receive_datetime = new.sent_datetime;
        else
            new.is_outed = false;
            new.receive_datetime = NULL;
            new.read_datetime = NULL;
        end if;
    end if;

    tableName = TG_RELNAME;
    localId = new.id;

    --учетные номера
    --вход€щий и исход€щий
    --если распор€жение в журнал попало от имени обычного пользовател€ (не jupiter !)
    --то мы должны поставить ему исход€щий номер, а вход€щий надо оставить пустым (но не NULL)
    --если от имени jupiter, то распор€жение €вл€етс€ вход€щим из удаленной системы и мы ему задаем вход€щий. исход€щий оставл€ем неизменным
    if(idUser = 2) then --jupiter
        new.input_number = generateUID(localId, tableName);
        --new.output_number = -- без изменений
    else
        new.output_number = generateUID(localId, tableName);
        new.input_number = '';
    end if;

    if(new.input_number isnull or new.output_number isnull) then
        raise exception 'Cannot generate input_number and output_number for the record! TABLE = %, LOCALID = %', tableName, localId;
        return NULL;
    end if;

    return new;

end$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgmsgjinsert', 'message_journal');

create trigger trgMsgJInsert
before insert or update
on message_journal 
for each row 
execute procedure msgJInsertCheck();
