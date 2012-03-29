select f_safe_drop_type('h_j_get_tsd_record');
create type h_j_get_tsd_record as(
                              id int4,
                              id_io_category int4,
                              insert_time timestamp,
                              id_dl_to int4,
                              id_dl_executor int4,
                              id_dl_controller int4,
                              exec_datetime timestamp,
                              exec_period int4[2],
                              osnovanie varchar,
                              is_archived bool,
                              id_jr_state int4
                              );
--Данная функция используется на клиенте для подмены запроса к данной таблице на получение информации об отдельной записи. 

create or replace function jGetTsdRecord(int4) returns setof h_j_get_tsd_record as
$BODY$
declare
    r h_j_get_tsd_record%rowtype;

    idRecord alias for $1;
    idDl int4;
    q varchar;
    privs varchar;

begin

    idDl = getCurrentDl();
    --если текущий пользователь не имеет права на чтение (2) ТСД, 
    --тогда разрешаем ему видеть только те записи табеля, 
    --в которых он (в роли текущего ДЛ) фигурирует как исполнитель, контроллер или получатель рез-тов
    if(getPrivilege(getCurrentUser(), 15, 2, true) = true) then
        privs = ' ';
    else
        privs = ' and (id_dl_to = ' || idDl || ' or id_dl_executor = ' || idDl || ' or id_dl_controller = ' || idDl || ')';
    end if;

    q = 'select 
            "id", 
            "id_io_category", 
            "insert_time", 
            "id_dl_to", 
            "id_dl_executor", 
            "id_dl_controller", 
            "exec_datetime", 
            "exec_period", 
            "osnovanie", 
            "is_archived", 
            "id_jr_state" 
        from tsd 
        where id = ' || idRecord || ' ' || privs;
        
    for r in execute q
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;
