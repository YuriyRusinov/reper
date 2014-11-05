CREATE OR REPLACE FUNCTION notifyGISLayer() RETURNS trigger AS
$BODY$
declare

    tableName varchar;
    recordUniqueId varchar;
    idRecord int8;
    whatHappens int2;

    payload varchar;

begin
    
    tableName := TG_RELNAME;
    if(substr(tableName, 1, 4) = 'tbl_') then
        tableName = substr(tableName, 5);
    end if;

                                                                                      
    if(TG_OP = 'INSERT') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        whatHappens := 1; --new EIO
    elsif(TG_OP = 'UPDATE') then
        idRecord = new.id;
        recordUniqueId = new.unique_id;
        whatHappens := 2; --updated EIO
    else
        idRecord = old.id;
        recordUniqueId = old.unique_id;
        whatHappens := 3; --updated EIO
    end if;

    payload = tableName || '~_~_~' || idRecord || '~_~_~' || recordUniqueId || '~_~_~' || whatHappens;
                               
    perform pg_notify('gis_data_changed', payload);

    return new;
end
$BODY$ 
language 'plpgsql' SECURITY DEFINER;

