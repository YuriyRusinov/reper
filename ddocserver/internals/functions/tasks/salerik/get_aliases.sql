CREATE OR REPLACE FUNCTION salerik.get_aliases() RETURNS SETOF record AS
'
   SELECT * FROM salerik.salerik_aliases;
'
LANGUAGE sql;

