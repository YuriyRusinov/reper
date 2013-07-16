CREATE OR REPLACE FUNCTION salerik.add_module(varchar) RETURNS integer  AS
$BODY$
BEGIN
    INSERT INTO salerik.salerik_modules (name) values ($1);
    RETURN 1;
END;
$BODY$
LANGUAGE 'plpgsql';
