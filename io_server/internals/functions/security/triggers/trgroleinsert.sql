create or replace function roleInsertCheck() returns trigger as
$BODY$
declare
    uid int4;
begin
    
    if(new.role_name isnull) then
        --raise notice 'ROLE_NAME IS NULL';
        if(new.id isnull) then
            select getNextSeq('kks_roles', 'id') into uid;
            new.id := uid;
	end if;
	
	--raise notice 'END ROLE_NAME IS NULL. ID = %', new.id;
	
        new.role_name := 'kksrole_' || new.id;
    end if;

    if(TG_OP = 'UPDATE') then
        --raise notice 'NEW.ID_JR_STATE = %', new.id_jr_state;
        if(new.id <> old.id) then
            raise exception 'You cannot change ID of kks_role!';
            return NULL;
        end if; 

        return new;
    end if;
    
    if(TG_RELNAME <> 'kks_roles' and TG_OP = 'INSERT') then
        --Этот костыль нужен для того, чтобы иметь возможность сылаться (REFERENCE) на таблицу kks_roles. 
        --В постгресе при наследовании таблиц отсутствует возможность ссылки, если данные ЯВНО не находятся в наследуемой таблице
        --Поэтому при записи в наследующие таблице необходимо продублировать запись и в наследуемую
        insert into kks_roles (id, role_name, with_inheritance) values (new.id, new.role_name, new.with_inheritance);
    end if;

    return new;

end
$BODY$
language 'plpgsql';

select f_safe_drop_trigger('trgroleinsert', 'kks_roles');

create trigger trgRoleInsert
before insert or update
on kks_roles 
for each row 
execute procedure roleInsertCheck();

select f_safe_drop_trigger('trgroleinsert', 'users');

create trigger trgRoleInsert
before insert or update
on users 
for each row 
execute procedure roleInsertCheck();

select f_safe_drop_trigger('trgroleinsert', 'position');

create trigger trgRoleInsert
before insert or update
on "position"
for each row 
execute procedure roleInsertCheck();

select f_safe_drop_trigger('trgroleinsert', 'units');

create trigger trgRoleInsert
before insert or update
on units
for each row 
execute procedure roleInsertCheck();
