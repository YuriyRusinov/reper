/*
CREATE FUNCTION plpgsql_call_handler () RETURNS LANGUAGE_HANDLER AS
    'plpgsql.dll' LANGUAGE C;
CREATE TRUSTED PROCEDURAL LANGUAGE plpgsql
    HANDLER plpgsql_call_handler;	
*/