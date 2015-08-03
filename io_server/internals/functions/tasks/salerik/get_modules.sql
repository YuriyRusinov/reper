CREATE OR REPLACE FUNCTION salerik.get_modules() RETURNS SETOF record AS
'
    SELECT * FROM salerik.salerik_modules;
'
LANGUAGE sql;

