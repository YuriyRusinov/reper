create or replace function generateUID(int8, varchar) returns varchar as
$BODY$
declare
    localId alias for $1;
    tableName alias for $2;

    orgUID varchar;
    theUID varchar;
    tName varchar;
    r record;
begin
    
    if(localId isnull or localId < 1 or tableName isnull) then
        return NULL;
    end if;

    orgUID = NULL;

    for r in
        select the_uid from getLocalOrg()
    loop
        orgUID := r.the_uid;
    end loop;

    if(substr(tableName, 1, 4) = 'tbl_') then
        tName = substr(tableName, 5);
    else
        tName = tableName;
    end if;
    
    --raise warning 'UNIQUE ID = %', orgUID;
    if(orgUID isnull) then
        orgUID = 'localorg';
    end if;

    theUID = orgUID || '-' || tName || '-' || localId;

    return theUID;

end
$BODY$
language 'plpgsql';

create or replace function uidCheck() returns trigger as
$BODY$
declare
    localId int8;
    tableName varchar;
    UID varchar;
    idUser int4;
    idObject int4;
begin

    if(new.last_update isnull) then
        new.last_update = current_timestamp;
    end if;

    if(TG_OP = 'UPDATE') then
        if(new.unique_id <> old.unique_id) then

            idUser = getCurrentUser();
            if(idUser = 2 or idUser = 1) then --jupiter and admin
                                                --we allow jupiter and admin to change UID of records and IO
                                                --but that must suggested as undeclared functionality (hack)
                return new;
            else
                raise exception 'You cannot change unique_id field of the record!';
                return NULL;
            end if;

        end if;

        return new;
    end if;

    idUser = getCurrentUser();
    --raise notice 'CURRENT_USER = %', idUser;
    
    if(idUser = 2) then --jupiter (we insert the record that come from another organization)
        
        --raise notice 'OLD_UNIQUE_ID IS %', new.unique_id;
        if(new.unique_id is not null) then
            return new;
        end if;
    end if;
    
--    if(new.unique_id is not null) then
        --raise notice 'User manually set value to unique_id field. Ignored! Will set autogenerated value!';
--    end if;

    tableName = TG_RELNAME;--����� ������ ������ �� ����, ��������� sequence ����������������� ������ � ����������, ������� � tbl_

    if(new.id isnull) then --in ordinary case new.id canNOT be NULL
        select getNextSeq(tableName, 'id') into localId;
        new.id = localId;
    end if;

    localId = new.id;

    --select id into idObject from tbl_io_objects where table_name = tableName;-- system tables MUST have uid with localorg-XXX-XXX template
    --if(idObject < 300) then
    --    UID = 'localorg-' || tableName || '-' || localId;
    --else
        UID = generateUID(localId, tableName);
    --end if;

    if(UID isnull) then
        raise exception 'Cannot generate unique_id for the record! TABLE = %, LOCALID = %', tableName, localId;
        return NULL;
    end if;

    new.unique_id = UID;

    --� �������� ���� ��������, ����� �� �� �����, ������ ���������� UID. 
    --��� ��������� ������ ����� ��� �������� (�.�. � �.�. �������� ������� � �������� ������������)
    --����������� (� �.�. � � ������ ��������������� ������)
    --
    --������ ��� ��������� ��������� (������� ��������� ��� ����������� �������), � ����� ��� ���, ������� ������������ ���� �������� ���
    --������� ����������. 
    --��� �� �����, ��������� �� ���� code � ������� attributes ������ ���������� ������, �� ������� �����������, ��� ������ ���� � ����� ������ ����� ��������� ������ ���������� ��������.
    if(tableName = 'attributes') then
        --if(new.id >= 300) then
            if(new.code isnull) then
                new.code = new.unique_id;
            end if;
        --end if;    
    end if;

    return new;
end
$BODY$
language 'plpgsql' security definer;


--invoked when tables creation executed. In that case functions f_safe_drop_XXX, f_safe_create_XXX dows not exist yet.
create or replace function createTriggerUID(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    --select f_safe_drop_trigger('trgsetuid', tableName);

    query = 'create trigger trgSetUID
             before insert or update
             on ' || tableName || '
             for each row 
             execute procedure uidCheck();';

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';

--more safe function. Can be used when update database to newer version or on ordinary work
--MUST be created in separate file!!!!
--this file invoked in initdb.sql script
/*
create or replace function createTriggerUIDEx(varchar) returns int4 as
$BODY$
declare
    tableName alias for $1;
    query varchar;
begin

    select f_safe_drop_trigger('trgsetuid', tableName);

    query = 'create trigger trgsetuid
             before insert or update
             on ' || tableName || '
             for each row 
             execute procedure uidCheck();';

    execute query;

    return 1;
end
$BODY$
language 'plpgsql';
*/
