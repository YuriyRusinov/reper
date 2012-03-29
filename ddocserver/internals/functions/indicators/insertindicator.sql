create or replace function insertIndicator(int4, int4, int4, varchar, varchar, varchar, varchar) returns int4 as
$BODY$
declare
    idType alias for $1;
    idParent alias for $2;
    idAttrType alias for $3;
    iName alias for $4;
    iDescription alias for $5;
    tableName alias for $6;
    columnName alias for $7;

    idIndicator int4;
    
begin

    select getNextSeq('indicator', 'id') into idIndicator;
    
    insert into indicator(id, 
                          id_indicator_type, 
                          id_parent, 
                          id_a_type, 
                          name, 
                          description, 
                          table_name, 
                          column_name) 

                  values (idIndicator, 
                          idType, 
                          idParent, 
                          idAttrType, 
                          iName, 
                          iDescription, 
                          tableName, 
                          columnName); 

    return idIndicator;
end
$BODY$
language 'plpgsql';

