/*
  Function: rSetUrl(int4, varchar)
  Функция задает новый URL паспорту файла. Т.е. после вызова считается, что файл, описываемый в данном паспорте, хранится по заданному пути. 
  Должен задаваться путь ОТНОСИТЕЛЬНО пути к файловому хранилищу.
  
  Parameters:
 
  $1 - идентификатор паспорта растра
  
  $2 - новый URL
  
  Returns:
  1 при успехе
  
  0, если указан неверные исходные данные (все параметры обязательные)
  
  -1, если пользователь не имеет прав на обновление информации в заданном файловом хранилище.
  
  -2, если указан неверный идентификатор паспорта
*/
create or replace function rSetUrl(int4, varchar) returns int4 as
$BODY$
declare
	iid_file alias for $1;
	iurl alias for $2;
	
	r RECORD;
	tableName varchar;
	dyn_query varchar;
	
	idUrl int4;	
begin

	if(iurl isnull) then
		return 0;
	end if;

	select id into idUrl from io_urls where id = iid_file;
	if(idUrl isnull) then
            return -2;
        end if;
  
	update io_urls set url = iurl where id = idUrl;
	
	return  1;
  
end
$BODY$
language 'plpgsql' security definer;
