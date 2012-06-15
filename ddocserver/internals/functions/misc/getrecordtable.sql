create or replace function getRecordTable(int8) returns varchar as
$BODY$
declare
    idRecord alias for $1;

    tName1 varchar;
    tName2 varchar;


    idObject1 int4;
    idObject2 int4;

    pos int4;
begin

    SELECT p.relname into tName1
    FROM q_base_table q, pg_class p
    WHERE q.id = idRecord AND q.tableoid = p.oid;
    
    --в силу того, что мы "подменяем" таблицы представлениями и именно название представления записываем в поле table_name таблицы tbl_io_objects,
    --нам необходимо вернуть именно то название (таблицы или представления), которое записано в tbl_io_objects
    --поэтому мы должны проанализировать полученное в tName1 значение

    pos = position('tbl_' in tName1);
    if(pos == 1) then --если название физической таблицы начинается с tbl_ , то можно предположить, что в io_objects 
        tName2 = substring(tName1 from 5);
        if(char_length(tName2) = 0) then
            return tName1;
        end if;
    else --в этом случае в Io_objects хранится название физической таблицы (acl_secureTable() для нее не вызывалась)
        return tName1;
    end if;

    
    select id into idObject1 from tbl_io_objects where table_name = tName1; --именно так называется таблица. Но в tbl_io_objects может быть записано иное, поэтому делаем второй запрос
    select id into idObject2 from tbl_io_objects where table_name = tName2; --предположительно именно это записано в tbl_io_objects

    if(idObject1 is not null and idObject2 is not null) then
        return tName1;
    end if;

    if(idObject2 is not null) then
        return tName2;
    end if;

    return tName1;
end
$BODY$
language 'plpgsql' security definer;
