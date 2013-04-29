select f_safe_drop_type('h_get_cmd_conf');

create type h_get_cmd_conf as(full_address varchar, 
                              id int4, 
                              uid varchar, 
                              id_jr_state int4, 
                              accepted_datetime timestamp, 
                              receive_datetime timestamp,
                              port int4);

create or replace function uGetCmdsConfirm() returns setof h_get_cmd_conf as
$BODY$
declare
    r h_get_cmd_conf%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;
    
    for r in
        select 
            --(uGetAddress(o.address, 0) || uGetAddress(p1.address, 1)) as full_address,
            (select address from uGetAddressEx(cmd.id_dl_from, idTransport)) as full_address,
            cmd.id,
            cmd.unique_id,
            cmd.id_jr_state,
            cmd.accepted_datetime,
            cmd.receive_datetime,
            (select port from uGetAddressEx(cmd.id_dl_from, idTransport)) as port

        from
            command_journal cmd,
            cmd_confirmations cfm
        where
            cfm.id_cmd = cmd.id
            and isLocalDl(cmd.id_dl_from) = FALSE
    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
        
end
$BODY$
language 'plpgsql';

create or replace function uGetCmdsConfirm1() returns setof h_get_cmd_conf as
$BODY$
declare
    r h_get_cmd_conf%rowtype;
    idTransport int4;
begin

    idTransport := getCurrentTransport();
    if(idTransport is null) then
        return;
    end if;

    for r in
        select 
            --(uGetAddress(o.address, 0) || uGetAddress(p1.address, 1)) as full_address,
            (select address from uGetAddressEx(cmd.id_dl_to, idTransport)) as full_address,
            cmd.id,
            cmd.unique_id,
            cmd.id_jr_state,
            cmd.accepted_datetime,
            cmd.receive_datetime,
            (select port from uGetAddressEx(cmd.id_dl_to, idTransport)) as port

        from
            command_journal cmd,
            cmd_confirmations cfm
        where
            cfm.id_cmd = cmd.id
            and isLocalDl(cmd.id_dl_to) = FALSE

    loop
        if(r.full_address is not null) then
            return next r;
        end if;
    end loop;

    return;
        
end
$BODY$
language 'plpgsql';
