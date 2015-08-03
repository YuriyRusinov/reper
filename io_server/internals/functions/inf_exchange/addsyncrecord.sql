create or replace function addSyncRecord(int4, int8, varchar, varchar, varchar, int4, int4) returns int8 as
$BODY$
declare
    idOrganization alias for $1;
    idEntity alias for $2;
    entityUID alias for $3;
    entityIOUID alias for $4;
    entityTable alias for $5;
    syncType alias for $6; -- 1 - always insert new
                           -- 2 - update if exist equal
                           -- 3 - delete if exist
    entityType alias for $7;  -- 1 - io_category
                              -- 2 - io_objects
                              -- 3 - records in main table of IO
                              -- 4 - records in addition tables of IO
                              -- 5 - addition tables of IO
                              -- 6,7,8,9 - system values (sync for organizations ONLY!)
                              -- 10 - public DL (creation, updating)
                              -- 11 - public DL (deleting)
                              -- 12 - files (transferred by parts) (creation, updating)
                              -- 13 - files (transferred by parts) (deleting)

    idRecord int8;
begin


    select into idRecord getNextSeq('out_sync_queue', 'id');

    insert into out_sync_queue (id, 
                                id_organization, 
                                id_entity,
                                entity_uid, 
                                entity_io_uid,
                                entity_table, 
                                entity_type, 
                                sync_type, 
                                sync_result)
                        values (idRecord, 
                                idOrganization, 
                                idEntity,
                                entityUID,
                                entityIOUID,
                                entityTable,  
                                entityType, 
                                syncType, 
                                1);

    if(FOUND = FALSE) then
        raise warning 'NEW RECORD IN TABLE OUT_SYNC_QUEUE WAS NOT CREATED!';
    end if;
    
    return idRecord;
end
$BODY$
language 'plpgsql';

--DO NOT CREATED FOR ORGANIZATION TABLE!!
-- FOR THAT PURPOSE USED SEPARATE TRIGGER FUNCTION (syncOrganizations())
create or replace function syncRecords() returns trigger as 
$BODY$
declare
    theTable varchar;
    idEntity int8;
    entityUID varchar;
    entityIOUID varchar;
    syncType int4;
    idObject int4;

    eType int4;--� ������� ���������� �������� ������� ��������� �� ������ ������ ������� ������� �� ���������
               --���������������, ��� � ����������� �� ���� ���������������� �������� (��, ���������, ������ �����������) 
               --�������� �����������, ���� ���� �������� ������, ����� ������� �� ������ ����
               --��� ���� ��������� ������ �������� �� ����������������
               -- 1 - ��
               -- 2 - ���������
               -- 3 - ������ �����������

    entityType int4;

    r record;
    ioFound bool;

    idSyncType int4;
    idOrganization int4;

    --���� ������������ ������ ����������� �������� ������ �� ���������� ��,
    --�� ��������������� �� ���� ����� ��������� (��� ���������� �� ������������� �� ����������������!)
    --����� ����, ���� �� ������ ����������������
    idRefObject int4;--������������� ����� �� (�����)
    idRefObjectOld int4; --������������� �� (������), �� ������� �� ��������� ����� ���������� ������ (������ ��� �������� UPDATE)
    refObjectUID varchar;--unique_id ����� ��
    bSendRefIO bool; --���� �� ���������� ��. � ������� � ������ �������� UPDATE, ���� �������� ��������, ������������ �� io_objects (�������� id_io_object), �� ��������, �� � �� ���������� �� ����.
    idRefCategory int4; --��������� ������� ����� ��
    refColumnName varchar; --�������, ����������� �� ��
begin

    idRefObject = -1;
    idRefObjectOld = -1;
    refObjectUID = '';
    bSendRefIO = false;
    refColumnName = '';

    theTable := TG_RELNAME;
    --if(theTable = 'chains_data')
    
    --if syncronization is stopped
    --or current user is jupiter
    if(processSync() = FALSE ) then --or getCurrentUser() = 2) then
        if(TG_OP = 'DELETE') then
            return old;
        else
            return new;
        end if;
    end if;

    theTable := TG_RELNAME;
    if(substr(theTable, 1, 4) = 'tbl_') then
        theTable = substr(theTable, 5);
    end if;

    --��������� �� ����������� � ����������� ��������, ������������ �� io_objects
    if(TG_OP = 'INSERT' or TG_OP = 'UPDATE') then
        for r in 
            select io.id, io.unique_id, io.id_io_category from f_sel_io_objects_tbl(theTable) io where io.table_name = theTable
        loop
            if(r.id < 99) then --�������, ��� ����� ��������� ������������ ������ chains_data �, �������� �������, �������� ������ �� ��
                continue; --���������� ������ �� ����� (�� ������������ ������ 1 ���)
            end if;

            --���������� �������� ��������� �������� �����������, ����� ������, �������� �� ��� ������� ���� ������ �� ��
            select id_child into idRefCategory from io_categories where id = r.id_io_category;
            if(idRefCategory isnull) then
                continue;
            end if;

            --�������� �� ������� ��������-������ �� ��
            select a.code into refColumnName from attributes a, attrs_categories ac where ac.id_io_category = idRefCategory and ac.id_io_attribute = a.id and a.table_name = 'io_objects';
            if(refColumnName is not null) then
                --!!!! -- ������ ������!!! ������ ����������� �������� ��������� ��������� NEW
                --����� �� ������ ������������� ��, �� ������� ��������� ������� ������ �������� �����������
                EXECUTE 'SELECT ($1)."' || refColumnName || '"::int4' INTO idRefObject USING NEW;

                --���� �������� UPDATE, �� ����� ��� � ���������� �������� ��������-������
                if(TG_OP = 'UPDATE') then
                    EXECUTE 'SELECT ($1)."' || refColumnName || '"::int4' INTO idRefObjectOld USING OLD;
                    if(idRefObject isnull) then
                        bSendRefIO = false;
                        idRefObject = -1;
                    elsif(idRefObject is not null and idRefObjectOld is not null and idRefObject = idRefObjectOld) then
                        bSendRefIO = false;
                        idRefObject = -1;
                    else
                        bSendRefIO = true;
                    end if;
                else --��� ���� �������� INSERT (DELETE ����� ���� �� �����)
                    if(idRefObject isnull) then
                        bSendRefIO = false;
                        idRefObject = -1;
                    else
                        bSendRefIO = true;
                    end if;
                end if;

                --���� ��������� ���������� ��, �� ������� ���� ������, �� ����� ��� ��� unique_id
                if(bSendRefIO = true) then
                    select io.unique_id into refObjectUID from f_sel_io_objects(idRefObject) io where io.id = idRefObject;
                end if;
                
            end if;
        end loop;
        
    end if;


    if(TG_OP = 'INSERT') then
        --����������� �� � ��������� �������������� �� ������� ������������� ����� ������� ������ ��� �������� update
        --����� ����� ���������� ���� is_completed � 1 ��� 2
        if(theTable = 'io_objects' or theTable = 'io_categories') then
            return new;
        end if;
        
        syncType = 1;
        idEntity = new.id;
        entityUID = new.unique_id;

    elsif(TG_OP = 'UPDATE') then --��������� ������ �����������
        if(old.id <> new.id) then
            raise notice 'You cannot change id of the records!';
            return NULL;
        end if;

        if(theTable = 'io_categories') then
            if(new.is_system = TRUE) then
                return new;
            end if;
        end if;

        if(theTable = 'io_objects' or theTable = 'io_categories') then
            if(new.is_completed = 0) then --�� ��� ��������� ��� �� ��������� ������� � ��
                return new;
            elsif(new.is_completed = 1 and old.is_completed = 0) then --�������� ����� �� ��� ���������
                syncType = 1;
                idEntity = new.id;
                entityUID = new.unique_id;
            elsif(new.is_completed = 2 and old.is_completed = 0) then --��������� ��������� ��������� �� ��� ���������
                syncType = 2;
                idEntity = new.id;
                entityUID = new.unique_id;
            else
                return new;
            end if;
        else  
            syncType = 2;
            idEntity = new.id;
            entityUID = new.unique_id;
        end if;

    else --�������� ������ �����������
        syncType = 3;
        idEntity = old.id;
        entityUID = old.unique_id;
    end if;

    if(theTable = 'io_objects') then
        idObject = idEntity;
        eType = 1; --objects
        if(TG_OP = 'INSERT' or TG_OP = 'UPDATE') then
            entityIOUID = new.unique_id;
        else
            entityIOUID = old.unique_id;
        end if;

        entityType = 2;
    elsif(theTable = 'io_categories') then
        idObject = 6; --!!!
        eType = 1; --!!!
        entityType = 1;

        if(TG_OP = 'INSERT' or TG_OP = 'UPDATE') then
            entityIOUID = new.unique_id;
        else
            entityIOUID = old.unique_id;
        end if;

    elsif(theTable = 'object_ref_tables') then
        entityType = 5;
        eType = 1;

        if(TG_OP = 'INSERT') then
            idObject = new.id_io_object;
        elsif (TG_OP = 'UPDATE') then 
            idObject = new.id_io_object;
        else
            idObject = old.id_io_object;
        end if;

        select unique_id into entityIOUID from io_objects where id = idObject;
    else
        ioFound = FALSE;
        for r in 
            select io.id, io.unique_id, io.id_sync_type, io.id_owner_org from f_sel_io_objects_tbl(theTable) io where io.table_name = theTable
        loop
            
            idSyncType = r.id_sync_type;
            idOrganization = r.id_owner_org;
            if(idSyncType = 4) then -- global, integrated
                if(idOrganization = getLocalOrgId()) then --do not send data. All data must be sended to owner!
                    if(TG_OP = 'DELETE') then
                        return old;
                    end if;
                    return new;
                end if;
            end if;
            
            idObject = r.id;
            entityIOUID = r.unique_id;
            ioFound = TRUE;
            eType = 3; --records
            entityType = 3;-- other records in main table
        end loop;

        if(ioFound = FALSE) then
            for r in select id_io_object, unique_id from object_ref_tables where table_name = theTable
            loop     
                idObject = r.id_io_object;
                entityIOUID = r.unique_id; --that right!
                eType = 3; -- records
                entityType = 4; -- other records in addition tables
            end loop;
            
        end if;
    end if;

    for r in
        select id from getSyncOrganizations(idObject, eType)
    loop
        if(bSendRefIO = true) then
            perform addSyncRecord(r.id, idRefObject, refObjectUID, refObjectUID, 'io_objects', 2/*update id exist equal*/, 2/*sending IO*/);
        end if;
        
        perform addSyncRecord(r.id, idEntity, entityUID, entityIOUID, theTable, syncType, entityType);
    end loop;

    if(TG_OP = 'DELETE') then
        return old;
    end if;

    return new;

end
$BODY$
language plpgsql security definer;

