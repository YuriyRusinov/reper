create or replace function getCurrentMacLabel() returns maclabel as
$BODY$
declare
    m maclabel;
begin
    select current_setting('mac_session_label') into m;

    return m;

--    EXCEPTION 
--             when OTHERS
--             then 
--                   m.m1 = 0;
--                   m.m2 = 0;
--                   return m;
end
$BODY$
language 'plpgsql';

create or replace function getCurrentMacLabelName() returns varchar as
$BODY$
declare
    n varchar;
    m maclabel;
begin
    m := getCurrentMacLabel();
    select name into n from maclabels where mac_value = m;
    if(FOUND = NULL) then
        return 'Not defined';
    end if;

    return n;
end
$BODY$
language 'plpgsql';

create or replace function getCurrentMacLabelId() returns int4 as
$BODY$
declare
    n int4;
    m maclabel;
begin
    m := getCurrentMacLabel();
    select id into n from maclabels where mac_value = m;
    if(FOUND = NULL) then
        return -1;
    end if;

    return n;
end
$BODY$
language 'plpgsql';

create or replace function getCapableSetMac() returns bool as
$BODY$
declare
    answer varchar;
begin
    select current_setting('mac_capable_setmac') into answer;
    if(answer = 'yes') then
        return TRUE;
    end if;

    return false;

    EXCEPTION 
             when OTHERS
             then 
                   return false;

end
$BODY$
language 'plpgsql';

create or replace function setCurrentMaclabel(int4) returns int4 as
$BODY$
declare
    newIdMac alias for $1;
    newMacS varchar;
    m maclabel;
begin

    if(getCapableSetMac() = TRUE) then
        
        select mac_value into m from maclabels where id = newIdMac;
        if(m isnull) then
            return 0;
        end if;
        select maclabel(m) into newMacS;
        select set_config('mac_session_label', newMacS, false);
        return 1;
    end if;

    return 0;

end
$BODY$
language 'plpgsql';

create or replace function canChangeMac(int4) returns bool as
$BODY$
declare
    newIdMac alias for $1;
    m1 maclabel;
    m2 maclabel;

    m_min maclabel;
    m_max maclabel;
begin

    select mac_value into m1 from maclabels where id = newIdMac;
    if(m1 isnull) then
        return false;
    end if;

    select current_setting('mac_user_label_max') into m_max;
    select current_setting('mac_user_label_min') into m_min;

    if(lt(m1, m_min) = TRUE or gt(m1, m_max) = TRUE) then
        return false;
    end if;

    if(getCapableSetMac() = FALSE) then
        return false;
    end if;

    m2 := getCurrentMacLabel();

    if(lt(m1, m2) = TRUE) then
        return false;
    end if;

    return true;

    EXCEPTION 
             when OTHERS
             then 
                   return false;


end
$BODY$
language 'plpgsql';
