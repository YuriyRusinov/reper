create or replace function ioInsertCheck() returns trigger as
$BODY$
declare
    idMaclabel int4;
--    m maclabel;
--    m1 maclabel;
    isGlobal bool;
    idChild int4;
begin
    
    if(TG_OP = 'UPDATE') then
        if(new.author <> old.author) then
            return NULL; 
        end if;
        
        if(new.is_global = false and old.is_global = true) then
            raise exception 'Change global object to nonglobal is not allowed!';
            return NULL;
        end if;
        if(new.id_owner_org <> old.id_owner_org) then
            raise exception 'You cannot change organization-owner of the object!';
            return NULL;
        end if;
        /*if(getCurrentUser() <> 1 and getCurrentUser() <> 2) then
            if(new.id_owner_org <> getLocalOrgId()) then
                raise exception 'ONLY OWNER ORGANIZATION CAN MODIFY THE OBJECT';
                return NULL;
            end if;
        end if;*/
    end if;

    if(new.id_owner_org isnull) then
        new.id_owner_org := getLocalOrgId();
    end if;

    if(new.is_global = true) then
        select is_global into isGlobal from io_categories where id = new.id_io_category;
        if(isGlobal isnull or isGlobal = false) then
            raise exception 'You cannot create global object on nonglobal category!';
            return NULL;
        end if;
    end if;

    if(new.id_io_type isnull) then
        new.id_io_type := 1;
    end if;
    
/*
    if(new.id_maclabel isnull) then
        m := getCurrentMacLabel();
        select id into idMaclabel from maclabels where mac_value = m;
        if(idMaclabel isnull) then
            raise exception 'ERROR! Current maclabel does not present in MACLABELS table!';
            return NULL;
        end if;
        new.id_maclabel = idMaclabel;
--        new."maclabel" := m;  --if uncomment then server crush!
    else
        m := getCurrentMacLabel();
        select mac_value into m1 from maclabels where id = new.id_maclabel;
        if(m1 isnull) then
            raise exception 'ERROR! Current maclabel does not present in MACLABELS table!';
            return NULL;
        end if;

        if(lt(m1, m) = TRUE) then --doc maclabel less then current
            raise exception 'ERROR! You cannot change down mandatory access level of the document!';
            return NULL;
	end if;
   	
        --greather or equal then do nothing 

	-- trigger ARTER INSERT will check if the user can change up level of document. 
	--If not then rollback
	
    end if;
*/
    if(getCurrentUser() <> 1 and getCurrentUser() <> 2) then --not admin and jupiter
        new.information := getAuthorInfo();
    end if;
    
    if(new.information isnull) then
        new.information := getAuthorInfo();
    end if;
    raise warning 'information is %', new.information;

    --������ ���� ���� ��� ��������� �� ��������-after, ����� �������� ������ � ���������� �������� �� ���� table_name
    if(TG_OP = 'INSERT') then
        select into idChild id_child from io_categories c where c.id = new.id_io_category;

        if (new.table_name is null and idChild is null) then
            return new;
        elsif (new.table_name is not null and idChild is null ) then
            raise exception 'Invalid table parameters';
            return NULL;
        elsif (new.table_name is null and idChild is not null) then
            new.table_name := 'eio_table_' || new.id;
        end if;

        new.table_name = getRealTableName(new.table_name, new.id);
        if(new.table_name is null) then
            raise exception 'Cannot generate table with given name! -- %', new.table_name;
            return NULL;
        elsif (new.id_io_type is null or new.id_io_type=1) then
            new.id_io_type := 2;
        end if;

    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgioinsert', 'io_objects');

select f_create_trigger('trgioinsert', 'before', 'insert or update', 'io_objects', 'ioinsertcheck()');

/*
create trigger trgIOInsert 
before insert or update
on tbl_io_objects 
for each row 
execute procedure ioInsertCheck();
*/

create or replace function ioInsertCheckAfter() returns trigger as
$BODY$
declare
    idMaclabel int4;
--    m maclabel;
--    m1 maclabel;
    
    capChmac varchar;
    cnt int4;
begin

    if(new.id_maclabel isnull) then
        return new;
    end if;
/*    
    m := getCurrentMacLabel();
    select mac_value into m1 from maclabels where id = new.id_maclabel;
    if(m1 isnull) then
        raise exception 'ERROR! Current maclabel does not present in MACLABELS table!';
        return NULL;
    end if;
    
    if(gt(m1, m) = TRUE) then --doc maclabel greather than current maclabel
	
        select current_setting('mac_capable_chmac') into capChmac;
	if(capChmac = 'yes') then
	    update io_objects set "maclabel" = m1 where id = new.id;
        else
	    raise exception 'ERROR! You does not have permissions to change up document maclabel relativelly your current access level!';
	    return NULL;
	end if;
    end if;
*/
    --if equal then do nothing


    if(new.id_sync_type = 4 )  then -- global, integrated
        if(new.id_owner_org <> getLocalOrgId()) then
            select count(*) into cnt from io_objects_organization where id_io_objects = new.id and id_organization = new.id_owner_org;
            if(cnt = 0) then
                insert into io_objects_organization (id_io_objects, id_organization) values (new.id, new.id_owner_org);
            end if;
        end if;
    end if;

    
    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('zz_trgzioinsertafter', 'io_objects');

select f_create_trigger('zz_trgzioinsertafter', 'after', 'insert or update', 'io_objects', 'ioinsertcheckafter()');

/*
create trigger zz_trgZIOInsertAfter
after insert or update
on tbl_io_objects
for each row
execute procedure ioInsertCheckAfter();
*/

create or replace function ioInsertCheckTableAfter() returns trigger as
$BODY$
declare
    table_name varchar;
    id_io_category int4;
    id_io_child_category int4;

    real_table_name varchar;
    id_obj int4;
    id_log int4;
    query varchar;
begin
    table_name := new.table_name;
    id_io_category := new.id_io_category;
    id_obj := new.id;

    if (new.table_name is null) then
        return new;
    end if;

    select into id_io_child_category id_child from io_categories c where c.id = new.id_io_category;
   
    --� ��������, ������� �� before �������������� �������� ������������ �������� ����������� �������
    --� ���������� �� ����������� �������� ��� �������������
    --����� ���� ��� ��� ���, ������ ��������� ��������� ������ � ���������� �������� �� ������ ����, �� 
    --���� ���� ���� ��� ��������� � �������-before

    if(new.id_search_template is not null and new.ref_table_name is not null) then
        if(isApplicable(new.id_search_template, id_io_child_category) = FALSE) then
            raise exception 'Given search template is not applicable for tables created on the current table category! ID_SEARCH_TEMPLATE = %, ID_CHILD_CATEGORY = %', new.id_search_template, id_io_child_category;
            return NULL;
        end if;

        real_table_name = createIOView (table_name, id_io_child_category, new.id_search_template, new.ref_table_name);
        table_name = real_table_name;
    else
        new.id_search_template = NULL;
        new.ref_table_name = NULL;
        real_table_name = createIOTable (table_name, id_io_child_category, TRUE);
        table_name = real_table_name;
    end if;
    
    if (table_name is null) then
        raise exception 'Cannot create table or view %', table_name;
        return NULL;
    end if;

    if((new.table_name isnull and table_name is not null) or table_name <> new.table_name) then
        query := 'update io_objects set table_name = ' || quote_literal (table_name) || ' where id = ' || id_obj;
        select into id_log setAsNotLogging (2);
        execute query;
        select into id_log setAsLogging (2);
    end if;
    
    new.table_name = table_name;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('zz_trgzioinserttableafter', 'io_objects');

select f_create_trigger('zz_trgzioinserttableafter', 'after', 'insert', 'io_objects', 'ioinsertchecktableafter()');
