/*ERRORS AND GROUP_ERRORS*/

/*SUCCESS*/
insert into group_error (id_group, name) values (1, 'SUCCESSFUL COMPLETION');

insert into error (sqlstate, id_group, name) values('00000', 1, 'SUCCESSFUL COMPLETION');

/*SQL STATEMENT NOT YET COMPLETE*/
insert into group_error (id_group, name) values (2, 'SQL STATEMENT NOT YET COMPLETE');

insert into error (sqlstate, id_group, name) values('03000', 2, 'SQL STATEMENT NOT YET COMPLETE');

/*CONNECTION EXCEPTION*/
insert into group_error (id_group, name) values (3, 'CONNECTION EXCEPTION');

insert into error (sqlstate, id_group, name) values('08000', 3, 'CONNECTION EXCEPTION');
insert into error (sqlstate, id_group, name) values('08003', 3, 'CONNECTION DOES NOT EXIST');
insert into error (sqlstate, id_group, name) values('08006', 3, 'CONNECTION FAILURE');
insert into error (sqlstate, id_group, name) values('08001', 3, 'SQLCLIENT UNABLE TO ESTABLISH SQLCONNECTION');
insert into error (sqlstate, id_group, name) values('08004', 3, 'SQLSERVER REJECTED ESTABLISHMENT OF SQLCONNECTION');
insert into error (sqlstate, id_group, name) values('08007', 3, 'TRANSACTION RESOLUTION UNKNOWN');
insert into error (sqlstate, id_group, name) values('08P01', 3, 'PROTOCOL VIOLATION');

/*TRIGGERED ACTION EXCEPTION*/
insert into group_error (id_group, name) values (4, 'TRIGGERED ACTION EXCEPTION');

insert into error (sqlstate, id_group, name) values('09000', 4, 'TRIGGERED ACTION EXCEPTION');

/*FEATURE NOT SUPPORTED*/
insert into group_error (id_group, name) values (5, 'FEATURE NOT SUPPORTED');

insert into error (sqlstate, id_group, name) values('0A000', 5, 'FEATURE NOT SUPPORTED');

/*INVALID TRANSACTION INITIATION*/
insert into group_error (id_group, name) values (6, 'INVALID TRANSACTION INITIATION');

insert into error (sqlstate, id_group, name) values('0B000', 6, 'INVALID TRANSACTION INITIATION');

/*LOCATOR EXCEPTION*/
insert into group_error (id_group, name) values (7, 'LOCATOR EXCEPTION');

insert into error (sqlstate, id_group, name) values('0F000', 7, 'LOCATOR EXCEPTION');
insert into error (sqlstate, id_group, name) values('0F001', 7, 'INVALID LOCATOR SPECIFICATION');

/*INVALID GRANTOR*/
insert into group_error (id_group, name) values (8, 'INVALID GRANTOR');

insert into error (sqlstate, id_group, name) values('0L000', 8, 'INVALID GRANTOR');
insert into error (sqlstate, id_group, name) values('0LP01', 8, 'INVALID GRANT OPERATION');

/*INVALID ROLE SPECIFICATION*/
insert into group_error (id_group, name) values (9, 'INVALID ROLE SPECIFICATION');

insert into error (sqlstate, id_group, name) values('0P000', 9, 'INVALID ROLE SPECIFICATION');

/*CARDINALITY VIOLATION*/
insert into group_error (id_group, name) values (10, 'CARDINALITY VIOLATION');

insert into error (sqlstate, id_group, name) values('21000', 10, 'CARDINALITY VIOLATION');

/*DATA EXCEPTION*/
insert into group_error (id_group, name) values (11, 'DATA EXCEPTION');

insert into error (sqlstate, id_group, name) values('22000', 11, 'DATA EXCEPTION');
insert into error (sqlstate, id_group, name) values('2202E', 11, 'ARRAY SUBSCRIPT ERROR');
insert into error (sqlstate, id_group, name) values('22021', 11, 'CHARACTER NOT IN REPERTOIRE');
insert into error (sqlstate, id_group, name) values('22008', 11, 'DATETIME FIELD OVERFLOW');
insert into error (sqlstate, id_group, name) values('22012', 11, 'DIVISION BY ZERO');
insert into error (sqlstate, id_group, name) values('22005', 11, 'ERROR IN ASSIGNMENT');
insert into error (sqlstate, id_group, name) values('2200B', 11, 'ESCAPE CHARACTER CONFLICT');
insert into error (sqlstate, id_group, name) values('22022', 11, 'INDICATOR OVERFLOW');
insert into error (sqlstate, id_group, name) values('22015', 11, 'INTERVAL FIELD OVERFLOW');
insert into error (sqlstate, id_group, name) values('2201E', 11, 'INVALID ARGUMENT FOR LOGARITHM');
insert into error (sqlstate, id_group, name) values('2201F', 11, 'INVALID ARGUMENT FOR POWER FUNCTION');
insert into error (sqlstate, id_group, name) values('2201G', 11, 'INVALID ARGUMENT FOR WIDTH BUCKET FUNCTION');
insert into error (sqlstate, id_group, name) values('22018', 11, 'INVALID CHARACTER VALUE FOR CAST');
insert into error (sqlstate, id_group, name) values('22007', 11, 'INVALID DATETIME FORMAT');
insert into error (sqlstate, id_group, name) values('22019', 11, 'INVALID ESCAPE CHARACTER');
insert into error (sqlstate, id_group, name) values('2200D', 11, 'INVALID ESCAPE OCTET');
insert into error (sqlstate, id_group, name) values('22025', 11, 'INVALID ESCAPE SEQUENCE');
insert into error (sqlstate, id_group, name) values('22P06', 11, 'NONSTANDARD USE OF ESCAPE CHARACTER');
insert into error (sqlstate, id_group, name) values('22010', 11, 'INVALID INDICATOR PARAMETER VALUE');
insert into error (sqlstate, id_group, name) values('22020', 11, 'INVALID LIMIT VALUE');
insert into error (sqlstate, id_group, name) values('22023', 11, 'INVALID PARAMETER VALUE');
insert into error (sqlstate, id_group, name) values('2201B', 11, 'INVALID REGULAR EXPRESSION');
insert into error (sqlstate, id_group, name) values('22009', 11, 'INVALID TIME ZONE DISPLACEMENT VALUE');
insert into error (sqlstate, id_group, name) values('2200C', 11, 'INVALID USE OF ESCAPE CHARACTER');
insert into error (sqlstate, id_group, name) values('2200G', 11, 'MOST SPECIFIC TYPE MISMATCH');
insert into error (sqlstate, id_group, name) values('22004', 11, 'NULL VALUE NOT ALLOWED');
insert into error (sqlstate, id_group, name) values('22002', 11, 'NULL VALUE NO INDICATOR PARAMETER');
insert into error (sqlstate, id_group, name) values('22003', 11, 'NUMERIC VALUE OUT OF RANGE');
insert into error (sqlstate, id_group, name) values('22026', 11, 'STRING DATA LENGTH MISMATCH');
insert into error (sqlstate, id_group, name) values('22001', 11, 'STRING DATA RIGHT TRUNCATION');
insert into error (sqlstate, id_group, name) values('22011', 11, 'SUBSTRING ERROR');
insert into error (sqlstate, id_group, name) values('22027', 11, 'TRIM ERROR');
insert into error (sqlstate, id_group, name) values('22024', 11, 'UNTERMINATED C STRING');
insert into error (sqlstate, id_group, name) values('2200F', 11, 'ZERO LENGTH CHARACTER STRING');
insert into error (sqlstate, id_group, name) values('22P01', 11, 'FLOATING POINT EXCEPTION');
insert into error (sqlstate, id_group, name) values('22P02', 11, 'INVALID TEXT REPRESENTATION');
insert into error (sqlstate, id_group, name) values('22P03', 11, 'INVALID BINARY REPRESENTATION');
insert into error (sqlstate, id_group, name) values('22P04', 11, 'BAD COPY FILE FORMAT');
insert into error (sqlstate, id_group, name) values('22P05', 11, 'UNTRANSLATABLE CHARACTER');

/*INTEGRITY CONSTRAINT VIOLATION*/
insert into group_error (id_group, name) values (12, 'INTEGRITY CONSTRAINT VIOLATION');

insert into error (sqlstate, id_group, name) values('23000', 12, 'INTEGRITY CONSTRAINT VIOLATION');
insert into error (sqlstate, id_group, name) values('23001', 12, 'RESTRICT VIOLATION');
insert into error (sqlstate, id_group, name) values('23502', 12, 'NOT NULL VIOLATION');
insert into error (sqlstate, id_group, name) values('23503', 12, 'FOREIGN KEY VIOLATION');
insert into error (sqlstate, id_group, name) values('23505', 12, 'UNIQUE VIOLATION');
insert into error (sqlstate, id_group, name) values('23514', 12, 'CHECK VIOLATION');

/*INVALID CURSOR STATE*/
insert into group_error (id_group, name) values (13, 'INVALID CURSOR STATE');

insert into error (sqlstate, id_group, name) values('24000', 13, 'INVALID CURSOR STATE');

/*INVALID TRANSACTION STATE*/
insert into group_error (id_group, name) values (14, 'INVALID TRANSACTION STATE');

insert into error (sqlstate, id_group, name) values('25000', 14, 'INVALID TRANSACTION STATE');
insert into error (sqlstate, id_group, name) values('25001', 14, 'ACTIVE SQL TRANSACTION');
insert into error (sqlstate, id_group, name) values('25002', 14, 'BRANCH TRANSACTION ALREADY ACTIVE');
insert into error (sqlstate, id_group, name) values('25008', 14, 'HELD CURSOR REQUIRES SAME ISOLATION LEVEL');
insert into error (sqlstate, id_group, name) values('25003', 14, 'INAPPROPRIATE ACCESS MODE FOR BRANCH TRANSACTION');
insert into error (sqlstate, id_group, name) values('25004', 14, 'INAPPROPRIATE ISOLATION LEVEL FOR BRANCH TRANSACTION');
insert into error (sqlstate, id_group, name) values('25005', 14, 'NO ACTIVE SQL TRANSACTION FOR BRANCH TRANSACTION');
insert into error (sqlstate, id_group, name) values('25006', 14, 'READ ONLY SQL TRANSACTION');
insert into error (sqlstate, id_group, name) values('25007', 14, 'SCHEMA AND DATA STATEMENT MIXING NOT SUPPORTED');
insert into error (sqlstate, id_group, name) values('25P01', 14, 'NO ACTIVE SQL TRANSACTION');
insert into error (sqlstate, id_group, name) values('25P02', 14, 'IN FAILED SQL TRANSACTION');

/*INVALID SQL STATEMENT NAME*/
insert into group_error (id_group, name) values (15, 'INVALID SQL STATEMENT NAME');

insert into error (sqlstate, id_group, name) values('26000', 15, 'INVALID SQL STATEMENT NAME');

/*TRIGGERED DATA CHANGE VIOLATION*/
insert into group_error (id_group, name) values (16, 'TRIGGERED DATA CHANGE VIOLATION');

insert into error (sqlstate, id_group, name) values('27000', 16, 'TRIGGERED DATA CHANGE VIOLATION');

/*INVALID AUTHORIZATION SPECIFICATION*/
insert into group_error (id_group, name) values (17, 'INVALID AUTHORIZATION SPECIFICATION');

insert into error (sqlstate, id_group, name) values('28000', 17, 'INVALID AUTHORIZATION SPECIFICATION');

/*DEPENDENT PRIVILEGE DESCRIPTORS STILL EXIST*/
insert into group_error (id_group, name) values (18, 'DEPENDENT PRIVILEGE DESCRIPTORS STILL EXIST');

insert into error (sqlstate, id_group, name) values('2B000', 18, 'DEPENDENT PRIVILEGE DESCRIPTORS STILL EXIST');
insert into error (sqlstate, id_group, name) values('2BP01', 18, 'DEPENDENT OBJECTS STILL EXIST');

/*INVALID TRANSACTION TERMINATION*/
insert into group_error (id_group, name) values (19, 'INVALID TRANSACTION TERMINATION');

insert into error (sqlstate, id_group, name) values('2D000', 19, 'INVALID TRANSACTION TERMINATION');

/*SQL ROUTINE EXCEPTION*/
insert into group_error (id_group, name) values (20, 'SQL ROUTINE EXCEPTION');

insert into error (sqlstate, id_group, name) values('2F000', 20, 'SQL ROUTINE EXCEPTION');
insert into error (sqlstate, id_group, name) values('2F005', 20, 'FUNCTION EXECUTED NO RETURN STATEMENT');
insert into error (sqlstate, id_group, name) values('2F002', 20, 'MODIFYING SQL DATA NOT PERMITTED');
insert into error (sqlstate, id_group, name) values('2F003', 20, 'PROHIBITED SQL STATEMENT ATTEMPTED');
insert into error (sqlstate, id_group, name) values('2F004', 20, 'READING SQL DATA NOT PERMITTED');

/*INVALID CURSOR NAME*/
insert into group_error (id_group, name) values (21, 'INVALID CURSOR NAME');

insert into error (sqlstate, id_group, name) values('34000', 21, 'INVALID CURSOR NAME');

/*EXTERNAL ROUTINE EXCEPTION*/
insert into group_error (id_group, name) values (22, 'EXTERNAL ROUTINE EXCEPTION');

insert into error (sqlstate, id_group, name) values('38000', 22, 'EXTERNAL ROUTINE EXCEPTION');
insert into error (sqlstate, id_group, name) values('38001', 22, 'CONTAINING SQL NOT PERMITTED');
insert into error (sqlstate, id_group, name) values('38002', 22, 'MODIFYING SQL DATA NOT PERMITTED');
insert into error (sqlstate, id_group, name) values('38003', 22, 'PROHIBITED SQL STATEMENT ATTEMPTED');
insert into error (sqlstate, id_group, name) values('38004', 22, 'READING SQL DATA NOT PERMITTED');

/*EXTERNAL ROUTINE INVOCATION EXCEPTION*/
insert into group_error (id_group, name) values (23, 'EXTERNAL ROUTINE INVOCATION EXCEPTION');

insert into error (sqlstate, id_group, name) values('39000', 23, 'EXTERNAL ROUTINE INVOCATION EXCEPTION');
insert into error (sqlstate, id_group, name) values('39001', 23, 'INVALID SQLSTATE RETURNED');
insert into error (sqlstate, id_group, name) values('39004', 23, 'NULL VALUE NOT ALLOWED');
insert into error (sqlstate, id_group, name) values('39P01', 23, 'TRIGGER PROTOCOL VIOLATED');
insert into error (sqlstate, id_group, name) values('39P02', 23, 'SRF PROTOCOL VIOLATED');

/*SAVEPOINT EXCEPTION*/
insert into group_error (id_group, name) values (24, 'SAVEPOINT EXCEPTION');

insert into error (sqlstate, id_group, name) values('3B000', 24, 'SAVEPOINT EXCEPTION');
insert into error (sqlstate, id_group, name) values('3B001', 24, 'INVALID SAVEPOINT SPECIFICATION');

/*INVALID CATALOG NAME*/
insert into group_error (id_group, name) values (25, 'INVALID CATALOG NAME');

insert into error (sqlstate, id_group, name) values('3D000', 25, 'INVALID CATALOG NAME');

/*INVALID SCHEMA NAME*/
insert into group_error (id_group, name) values (26, 'INVALID SCHEMA NAME');

insert into error (sqlstate, id_group, name) values('3F000', 26, 'INVALID SCHEMA NAME');

/*TRANSACTION ROLLBACK*/
insert into group_error (id_group, name) values (27, 'TRANSACTION ROLLBACK');

insert into error (sqlstate, id_group, name) values('40000', 27, 'TRANSACTION ROLLBACK');
insert into error (sqlstate, id_group, name) values('40002', 27, 'TRANSACTION INTEGRITY CONSTRAINT VIOLATION');
insert into error (sqlstate, id_group, name) values('40001', 27, 'SERIALIZATION FAILURE');
insert into error (sqlstate, id_group, name) values('40003', 27, 'STATEMENT COMPLETION UNKNOWN');
insert into error (sqlstate, id_group, name) values('40P01', 27, 'DEADLOCK DETECTED');

/*SYNTAX ERROR OR ACCESS RULE VIOLATION*/
insert into group_error (id_group, name) values (28, 'SYNTAX ERROR OR ACCESS RULE VIOLATION');

insert into error (sqlstate, id_group, name) values('42000', 28, 'SYNTAX ERROR OR ACCESS RULE VIOLATION');
insert into error (sqlstate, id_group, name) values('42601', 28, 'SYNTAX ERROR');
insert into error (sqlstate, id_group, name) values('42501', 28, 'INSUFFICIENT PRIVILEGE');
insert into error (sqlstate, id_group, name) values('42846', 28, 'CANNOT COERCE');
insert into error (sqlstate, id_group, name) values('42803', 28, 'GROUPING ERROR');
insert into error (sqlstate, id_group, name) values('42830', 28, 'INVALID FOREIGN KEY');
insert into error (sqlstate, id_group, name) values('42602', 28, 'INVALID NAME');
insert into error (sqlstate, id_group, name) values('42622', 28, 'NAME TOO LONG');
insert into error (sqlstate, id_group, name) values('42939', 28, 'RESERVED NAME');
insert into error (sqlstate, id_group, name) values('42804', 28, 'DATATYPE MISMATCH');
insert into error (sqlstate, id_group, name) values('42P18', 28, 'INDETERMINATE DATATYPE');
insert into error (sqlstate, id_group, name) values('42809', 28, 'WRONG OBJECT TYPE');
insert into error (sqlstate, id_group, name) values('42703', 28, 'UNDEFINED COLUMN');
insert into error (sqlstate, id_group, name) values('42883', 28, 'UNDEFINED FUNCTION');
insert into error (sqlstate, id_group, name) values('42P01', 28, 'UNDEFINED TABLE');
insert into error (sqlstate, id_group, name) values('42P02', 28, 'UNDEFINED PARAMETER');
insert into error (sqlstate, id_group, name) values('42704', 28, 'UNDEFINED OBJECT');
insert into error (sqlstate, id_group, name) values('42701', 28, 'DUPLICATE COLUMN');
insert into error (sqlstate, id_group, name) values('42P03', 28, 'DUPLICATE CURSOR');
insert into error (sqlstate, id_group, name) values('42P04', 28, 'DUPLICATE DATABASE');
insert into error (sqlstate, id_group, name) values('42723', 28, 'DUPLICATE FUNCTION');
insert into error (sqlstate, id_group, name) values('42P05', 28, 'DUPLICATE PREPARED STATEMENT');
insert into error (sqlstate, id_group, name) values('42P06', 28, 'DUPLICATE SCHEMA');
insert into error (sqlstate, id_group, name) values('42P07', 28, 'DUPLICATE TABLE');
insert into error (sqlstate, id_group, name) values('42712', 28, 'DUPLICATE ALIAS');
insert into error (sqlstate, id_group, name) values('42710', 28, 'DUPLICATE OBJECT');
insert into error (sqlstate, id_group, name) values('42702', 28, 'AMBIGUOUS COLUMN');
insert into error (sqlstate, id_group, name) values('42725', 28, 'AMBIGUOUS FUNCTION');
insert into error (sqlstate, id_group, name) values('42P08', 28, 'AMBIGUOUS PARAMETER');
insert into error (sqlstate, id_group, name) values('42P09', 28, 'AMBIGUOUS ALIAS');
insert into error (sqlstate, id_group, name) values('42P10', 28, 'INVALID COLUMN REFERENCE');
insert into error (sqlstate, id_group, name) values('42611', 28, 'INVALID COLUMN DEFINITION');
insert into error (sqlstate, id_group, name) values('42P11', 28, 'INVALID CURSOR DEFINITION');
insert into error (sqlstate, id_group, name) values('42P12', 28, 'INVALID DATABASE DEFINITION');
insert into error (sqlstate, id_group, name) values('42P13', 28, 'INVALID FUNCTION DEFINITION');
insert into error (sqlstate, id_group, name) values('42P14', 28, 'INVALID PREPARED STATEMENT DEFINITION');
insert into error (sqlstate, id_group, name) values('42P15', 28, 'INVALID SCHEMA DEFINITION');
insert into error (sqlstate, id_group, name) values('42P16', 28, 'INVALID TABLE DEFINITION');
insert into error (sqlstate, id_group, name) values('42P17', 28, 'INVALID OBJECT DEFINITION');

/*WITH CHECK OPTION VIOLATION*/
insert into group_error (id_group, name) values (29, 'WITH CHECK OPTION VIOLATION');

insert into error (sqlstate, id_group, name) values('44000', 29, 'WITH CHECK OPTION VIOLATION');

/*INSUFFICIENT RESOURCES*/
insert into group_error (id_group, name) values (30, 'INSUFFICIENT RESOURCES');

insert into error (sqlstate, id_group, name) values('53000', 30, 'INSUFFICIENT RESOURCES');
insert into error (sqlstate, id_group, name) values('53100', 30, 'DISK FULL');
insert into error (sqlstate, id_group, name) values('53200', 30, 'OUT OF MEMORY');
insert into error (sqlstate, id_group, name) values('53300', 30, 'TOO MANY CONNECTIONS');

/*PROGRAM LIMIT EXCEEDED*/
insert into group_error (id_group, name) values (31, 'PROGRAM LIMIT EXCEEDED');

insert into error (sqlstate, id_group, name) values('54000', 31, 'PROGRAM LIMIT EXCEEDED');
insert into error (sqlstate, id_group, name) values('54001', 31, 'STATEMENT TOO COMPLEX');
insert into error (sqlstate, id_group, name) values('54011', 31, 'TOO MANY COLUMNS');
insert into error (sqlstate, id_group, name) values('54023', 31, 'TOO MANY ARGUMENTS');

/*OBJECT NOT IN PREREQUISITE STATE*/
insert into group_error (id_group, name) values (32, 'OBJECT NOT IN PREREQUISITE STATE');

insert into error (sqlstate, id_group, name) values('55000', 32, 'OBJECT NOT IN PREREQUISITE STATE');
insert into error (sqlstate, id_group, name) values('55006', 32, 'OBJECT IN USE');
insert into error (sqlstate, id_group, name) values('55P02', 32, 'CANT CHANGE RUNTIME PARAM');
insert into error (sqlstate, id_group, name) values('55P03', 32, 'LOCK NOT AVAILABLE');

/*OPERATOR INTERVENTION*/
insert into group_error (id_group, name) values (33, 'OPERATOR INTERVENTION');

insert into error (sqlstate, id_group, name) values('57000', 33, 'OPERATOR INTERVENTION');
insert into error (sqlstate, id_group, name) values('57014', 33, 'QUERY CANCELED');
insert into error (sqlstate, id_group, name) values('57P01', 33, 'ADMIN SHUTDOWN');
insert into error (sqlstate, id_group, name) values('57P02', 33, 'CRASH SHUTDOWN');
insert into error (sqlstate, id_group, name) values('57P03', 33, 'CANNOT CONNECT NOW');

/*SYSTEM ERROR*/
insert into group_error (id_group, name) values (34, 'SYSTEM ERROR');

insert into error (sqlstate, id_group, name) values('58030', 34, 'IO ERROR');
insert into error (sqlstate, id_group, name) values('58P01', 34, 'UNDEFINED FILE');
insert into error (sqlstate, id_group, name) values('58P02', 34, 'DUPLICATE FILE');

/*PLPGSQL ERROR*/
insert into group_error (id_group, name) values (35, 'PLPGSQL ERROR');

insert into error (sqlstate, id_group, name) values('P0000', 35, 'PLPGSQL ERROR');
insert into error (sqlstate, id_group, name) values('P0001', 35, 'RAISE EXCEPTION');


/*KKSSITO ERROR*/
insert into group_error (id_group, name) values (36, 'KKSSITO ERROR');

insert into error (sqlstate, id_group, name) values('XG000', 36, 'KKSSITO ERROR');
