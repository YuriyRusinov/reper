
create or REPLACE FUNCTION salerik.add_alias(varchar, varchar,integer, boolean,varchar) RETURNS integer  AS
$BODY$
BEGIN
    INSERT INTO salerik.salerik_aliases (alias_name,alias_value,alias_parent,issystem,description) values ($1,$2, $3,$4,$5);
    RETURN 1;
END;
$BODY$
LANGUAGE 'plpgsql';

