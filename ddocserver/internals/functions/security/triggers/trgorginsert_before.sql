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

    if(TG_OP = 'UPDATE' and getCurrentUser() = 2) then --2 = jupiter
        --необходимо отследить ситуацию, когда администраторизменил тип нелокальной организации,
        --а по каналу инф.обмена пришло обновление информации об этой организации
        --¬ этом случае необходимо оставить предыдущий тип
        if(old.id_type_org = 3) then
            new.id_type_org = old.id_type_org;
        end if;
    end if;
    
    if(isMain <> 1 and getCurrentUser() <> 2 and new.id <> getLocalOrgId()) then --2 = jupiter
        select count(*) into isMain from organization;
        if(isMain = 0) then
            return new; --allow add first record in installer
        end if;
        
        --нам необходимо разрешить администратору системы мен€ть тип нелокальной организации на "внешн€€ сопр€гаема€ система"
        --при этом надо запретить смену всех других атрибутов
        if(TG_OP = 'UPDATE') then
            if(new.id_type_org <> old.id_type_org and new.id_type_org = 3) then
                new = old;
                new.id_type_org = 3;
                return new;
            end if;
            --return new;
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

