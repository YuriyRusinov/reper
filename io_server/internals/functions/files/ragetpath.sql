/*
Function: raGetPath()
Функция возвращает путь к файловому хранилищу относительно сервера БГД.
*/
create or replace function raGetPath() returns varchar as
$BODY$
declare
	opath varchar;
begin

	opath = getSystemParamStr(1);

	if(substring(opath from char_length(opath) for 1) <> '/') then
		opath := opath || '/';
	end if;
	
	return opath;
end
$BODY$
language 'plpgsql';
