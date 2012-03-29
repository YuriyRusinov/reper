select f_safe_drop_type('h_j_get_tsd');
create type h_j_get_tsd as(
                              id int4,
                              id_io_category varchar,
                              insert_time timestamp,
                              id_dl_to varchar,
                              id_dl_executor varchar,
                              id_dl_controller varchar,
                              exec_datetime timestamp,
                              exec_period int4[2],
                              osnovanie varchar,
                              is_archived bool,
                              id_jr_state varchar
                              );
--Данная функция используется на клиенте для подмены запроса к данной таблице. 
--Поэтому, исходя из логики работы клиента, вместо идентификаторов (внешних ключей)
--возвращаются строковые значения из соответствующих таблиц
--кроме того, в качестве параметра передается набор фильтров.
--Фильтры генерируются на клиенте в соответствии с логикой работы клиента

create or replace function jGetTsd(varchar) returns setof h_j_get_tsd as
$BODY$
declare
    r h_j_get_tsd%rowtype;
    q varchar;

    filters alias for $1;
    privs varchar;
    idDl int4;

begin

    idDl = getCurrentDl();
    --если текущий пользователь не имеет права на чтение (2) ТСД, 
    --тогда разрешаем ему видеть только те записи табеля, 
    --в которых он (в роли текущего ДЛ) фигурирует как исполнитель, контроллер или получатель рез-тов
    if(getPrivilege(getCurrentUser(), 15, 2, true) = true) then
        privs = ' ';
    else
        privs = ' and (tsd.id_dl_to = ' || idDl || ' or tsd.id_dl_executor = ' || idDl || ' or tsd.id_dl_controller = ' || idDl || ')';
    end if;

    q = 'select 
            distinct tsd.id , 
            io_categories."name" as "id_io_category", 
            tsd."insert_time", 
            position."name" as "id_dl_to", 
            position1."name" as "id_dl_executor", 
            position2."name" as "id_dl_controller", 
            tsd."exec_datetime", 
            tsd."exec_period", 
            tsd."osnovanie", 
            tsd."is_archived", 
            jr_states."name" as "id_jr_state" 
        from 
            tsd  
            left join io_categories on tsd."id_io_category" = io_categories.id 
            left join position on tsd."id_dl_to" = position.id 
            left join position position1 on tsd."id_dl_executor" = position1.id 
            left join position position2 on tsd."id_dl_controller" = position2.id 
            left join jr_states on tsd."id_jr_state" = jr_states.id 
        where 1=1 ';
        
    q = q || privs;

    if(filters is not null) then
        if(substr(filters, 1, 3) = 'and') then
            q = q || filters;
        else
            q = q || ' and ' || filters;
        end if;
    end if;

    for r in execute q
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql' security definer;
