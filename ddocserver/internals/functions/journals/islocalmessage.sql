-- OUT message is the message that must be sended to receiver
-- IN message is the message that  sender is local DL
create or replace function isLocalOutMessage(int4) returns boolean as
$BODY$
declare
    idDlReceiver alias for $1;
    bIsLocal bool;
begin

    select isLocalDl(idDlReceiver) into bIsLocal;
    
    if(bIsLocal isnull) then
        bIsLocal = TRUE;
    end if;    

    return bIsLocal;

end
$BODY$
language 'plpgsql';

create or replace function isLocalInMessage(int4) returns boolean as
$BODY$
declare
    idDlSender alias for $1;
    bIsLocal bool;
begin

    select isLocalDl(idDlSender) into bIsLocal;
    
    if(bIsLocal isnull) then
        bIsLocal = TRUE;
    end if;    

    return bIsLocal;

end
$BODY$
language 'plpgsql';
