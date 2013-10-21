create or replace function orgInsertCheckBefore() returns trigger as
$BODY$
declare
    r record;
    isMain int4;
    io_uid varchar;
begin

    select f_is_table_exist('main_org_table', NULL) into isMain;

    if(TG_OP = 'DELETE') then
        --delete organization can ONLY admin on main organization and system user jupiter
        if(isMain <> 1 and getCurrentUser() <> 2) then --jupiter
            raise exception 'Only administrator of main organization can delete this data!';
            return NULL;
        end if;

        if(getCurrentUser() <> 2 and old.id = getLocalOrgId()) then
            raise exception 'You cannot delete current local organization!';
            return NULL;
        end if;

        return old;

    end if;

    if(isMain <> 1 and getCurrentUser() <> 2 and new.id <> getLocalOrgId()) then --jupiter
        select count(*) into isMain from organization;
        if(isMain = 0) then
            return new; --allow add first record in installer
        end if;

        raise exception 'Only administrator of main organization can add or modify info about foreign organizations!';
        return NULL;
    end if;

    return new;
end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgorginsertbefore', 'organization');

create trigger trgOrgInsertBefore
before insert or update or delete
on organization
for each row 
execute procedure orgInsertCheckBefore();

