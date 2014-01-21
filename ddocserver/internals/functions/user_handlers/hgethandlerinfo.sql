select f_safe_drop_type('h_get_handler_info');
create type h_get_handler_info as (id_handler int4,
                                   name varchar,
                                   description varchar,
                                   service varchar,
                                   extra_params varchar,
                                   is_external boolean,
                                   h_host varchar,
                                   h_port int4);

create or replace function hGetHandlerInfo(int4) returns setof h_get_handler_info as
$BODY$
declare
    idChainData alias for $1;
    r h_get_handler_info%rowtype;
begin

    for r in 
        select h.id,
               h.name,
               h.description,
               hp.service,
               hp.extra_params,
               h.is_external,
               hp.h_host,
               hp.h_port
        from
            handlers h,
            handler_params hp,
            chains c,
            chains_data cd
        where
            cd.id = idChainData
            and cd.id_chain = c.id
            and c.id_handler = h.id
            and h.id_handler_params = hp.id
    loop
        update chains_data set handled_time = current_timestamp where id = idChainData;
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql';
