CREATE schema salerik;

CREATE table salerik.salerik_aliases  (id  SERIAL  PRIMARY KEY,alias_name varchar(128), alias_value varchar(128), alias_parent integer,issystem boolean,description varchar(256));
CREATE table salerik.alias_config  (id INTEGER REFERENCES salerik.salerik_aliases, conf_name varchar(128), conf_value varchar(256));
CREATE table salerik.components (id  SERIAL  PRIMARY KEY,component_name varchar(128), component_type varchar(128), component_value varchar(128),issystem boolean,description varchar(256));
CREATE table salerik.component_type (id  SERIAL  PRIMARY KEY,type_name varchar(128), type_id integer);

CREATE OR REPLACE FUNCTION salerik.get_aliases() RETURNS SETOF record AS'SELECT * FROM salerik.salerik_aliases;'LANGUAGE sql;
CREATE OR REPLACE FUNCTION salerik.add_alias(varchar, varchar,integer, boolean,varchar) RETURNS integer  AS'
BEGIN
INSERT INTO salerik.salerik_aliases (alias_name,alias_value,alias_parent,issystem,description) values ($1,$2, $3,$4,$5);
RETURN 1;
END;
'LANGUAGE plpgsql;

CREATE table salerik.salerik_modules (id  SERIAL  PRIMARY KEY,name varchar(128), issytem boolean,description varchar(256));
CREATE table salerik.modules_config  (id INTEGER REFERENCES salerik.salerik_modules, conf_name varchar(128), conf_value varchar(256));

CREATE OR REPLACE FUNCTION salerik.get_modules() RETURNS SETOF record AS'SELECT * FROM salerik.salerik_modules;'LANGUAGE sql;
CREATE OR REPLACE FUNCTION salerik.add_module(varchar) RETURNS integer  AS'
BEGIN
INSERT INTO salerik.salerik_modules (name) values ($1);
RETURN 1;
END;
'LANGUAGE plpgsql;
