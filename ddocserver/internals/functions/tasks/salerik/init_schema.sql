CREATE schema salerik;

CREATE table salerik.salerik_aliases  (id  SERIAL  PRIMARY KEY,
                                       alias_name varchar(128), 
                                       alias_value varchar(128), 
                                       alias_parent integer,
                                       issystem boolean,
                                       description varchar(256));

CREATE table salerik.alias_config  (id INTEGER REFERENCES salerik.salerik_aliases, 
                                    conf_name varchar(128), 
                                    conf_value varchar(256));

CREATE table salerik.components (id  SERIAL  PRIMARY KEY,
                                 component_name varchar(128), 
                                 component_type varchar(128), 
                                 component_value varchar(128),
                                 issystem boolean,
                                 description varchar(256));

CREATE table salerik.component_type (id  SERIAL  PRIMARY KEY,
                                     type_name varchar(128), 
                                     type_id integer);

CREATE table salerik.salerik_modules (id  SERIAL  PRIMARY KEY,
                                      name varchar(128), 
                                      issytem boolean,
                                      description varchar(256));

CREATE table salerik.modules_config  (id INTEGER REFERENCES salerik.salerik_modules, 
                                      conf_name varchar(128), 
                                      conf_value varchar(256));
