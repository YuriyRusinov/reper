create or replace function checkForPublic(int4, int4) returns bool as
$BODY$
declare
    idSender alias for $1;
    idReceiver alias for $2;
    
    senderIsPublic bool;
    receiverIsLocal bool;
begin

    
    receiverIsLocal = isLocalDl(idReceiver);
    if(receiverIsLocal = TRUE) then
        return TRUE;
    end if;
    
    senderIsPublic = isPublicDl(idSender);
    if(senderIsPublic = TRUE) then
        return TRUE;
    end if;
    
    return FALSE;
end
$BODY$
language 'plpgsql';

create or replace function checkForPublic(int4, int4, int4) returns bool as
$BODY$
declare
    idFrom alias for $1;
    idExec alias for $2;
    idTo alias for $3;
    
    fromIsPublic bool;
    execIsLocal bool;
    toIsLocal bool;
begin

    execIsLocal = isLocalDl(idExec);
    toIsLocal = isLocalDl(idTo);
    
    if(execIsLocal = FALSE or toIsLocal = false) then
        fromIsPublic = isPublicDl(idFrom);
        if(fromIsPublic <> TRUE) then
            return false;
        end if;
        fromIsPublic = isPublicDl(idExec);
        if(fromIsPublic <> TRUE) then
            return false;
        end if;
        fromIsPublic = isPublicDl(idTo);
        if(fromIsPublic <> TRUE) then
            return false;
        end if;
    end if;

    return true;
end
$BODY$
language 'plpgsql';
