/*
  Function: addGroup(name, varchar, boolean)

  Создание группы пользователей. 
  
  Если группа с таким названием уже существует в кластере баз данных, то она только добавляется в таблицу GROUPS.

  Parameters:
  $1 - название группы (обязательный параметр. Не может быть NULL)
  
  $2 - флаг наследования прав ролей, в которые входит данная группа (если TRUE, то должна наследовать)
  
  Returns:
  Идентификатор созданной группы.
  
  Если входные параметры указаны как NULL или при создании группы произошла ошибка, то функция генерирует исключение.
*/
CREATE OR REPLACE FUNCTION addGroup(name, boolean) RETURNS INTEGER AS
$BODY$
DECLARE
    gn ALIAS FOR $1;
    needInherit alias for $2;

    dyn_query VARCHAR;
    gid int4;
    res int4;
BEGIN
    if(gn isnull) then
	raise exception 'Input parameters cannot be NULL. Check input parameters!';
    end if;
    
    --создаем группу в таблице pg_group
    select safeCreateGroup(gn) into res;
    if(res = 0) then
	select count(groname) into res from pg_group where groname = gn;
	if(res = 1) then 
	    raise exception 'Clucter already contains ROLE with specified name. New GROUP must have another name!';
	end if;
    end if;
    
    RETURN res;
END;
$BODY$
LANGUAGE 'plpgsql';
