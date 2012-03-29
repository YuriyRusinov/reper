/*
select * from createTempTables();
select * from setCurrentDl(4);
*/

insert into xml_query_kinds (unique_id, name) values ('localorg-xml_query_kinds-1', 'Регламентированный');
insert into xml_query_kinds (unique_id, name) values ('localorg-xml_query_kinds-2', 'Не регламентированный');

INSERT INTO xml_query_types(name, code) VALUES ('Запрос событий с уровнем тревоги выше заданного', 'getNextEventsFullResponse(String filter)');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос перечня неисправных устройств ИК ТСО', 'getFaultyDevicesFullResponse()');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос отчета об охраняемых объектах', 'getSafeReportFullResponse()');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос графических планов  ИК ТСО', 'getGPlanSchemesFullResponse()');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос элементов графических планов', 'getGPlanSchemeElementsFullResponse()');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос отчета о персонале', 'getPersonalReportFullResponse(String filter)');
INSERT INTO xml_query_types(name, code) VALUES ('Запрос отчета о событиях с уровнем тревоги выше заданного', 'getEventReportFullResponse(String filter)');
--INSERT INTO xml_query_types(name, code) VALUES ('Запрос', 'getGPlanSchemesResponse');
--INSERT INTO xml_query_types(name, code) VALUES ('Запрос', 'getGPlanSchemeElementsResponse');
--INSERT INTO xml_query_types(name, code) VALUES ('Запрос', 'getNextGPlanSchemeElementsResponse');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 1,  0, '/Envelope/Body/getNextEventsFullResponse/events', 'event', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 1,  1, '/event', 'person', 'id');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 2,  0, '/Envelope/Body/getFaultyDevicesFullResponse/', 'faultyDevice', 'deviceId');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 3,  0, '/Envelope/Body/getSafeReportFullResponse/result', 'safe', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 3,  1, '/safe/devices', 'device', 'id');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 4,  0, '/Envelope/Body/getGPlanSchemesFullResponse/result', 'scheme', 'id');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 5,  0, '/Envelope/Body/getGPlanSchemeElementsFullResponse/result', 'element', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 5,  1, '/element/shapes', 'shape', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 5,  2, '/shape/segments', 'segment', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 5,  3, '/segment/points', 'point', '');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 6,  0, '/Envelope/Body/getPersonalReportFullResponse/result', 'person', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 6,  1, '/person/cards', 'card', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 6,  2, '/card/plans', 'plan', 'id');

INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 7,  0, '/Envelope/Body/getEventReportFullResponse/result', 'event', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 7,  1, '/event', 'person', 'id');

/*
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 8,  0, '/Envelope/Body/getGPlanSchemesResponse/result/schemes', 'scheme', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 9,  0, '/Envelope/Body/getGPlanSchemeElementsResponse/result/elements', 'element', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 9,  1, '/element/shapes', 'shape', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 9,  2, '/shape/segments', 'segment', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 9,  3, '/segment/points', 'point', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 10, 0, '/Envelope/Body/getNextGPlanSchemeElementsResponse/result/elements', 'element', 'id');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 10, 1, '/element/shapes', 'shape', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 10, 2, '/shape/segments', 'segment', '');
INSERT INTO xml_formats (id_xml_query_type, xml_level, xpath, node_name, id_name) VALUES ( 10, 3, '/segment/points', 'point', '');
*/

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'id', 'id_event', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'date', 'date', NULL, true, 'date', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'writeDate', 'writeDate', NULL, true, 'date', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'message', 'message', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'source', 'source', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'alarm', 'alarm', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'deviceId', 'deviceId', NULL, false, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'cardCode', 'cardCode', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'pass', 'pass', NULL, false, 'bool', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'direction', 'direction', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'fields', 'fields', NULL, false, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'videoURL', 'videoURL', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, 'id', 'person_id', NULL, false, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, '/fields/field/name', 'f_name', NULL, false, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 1, '/fields/field/value', 'f_value', NULL, false, 'varchar', 1);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 2, 'deviceId', 'deviceId', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 2, 'deviceName', 'deviceName', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 2, 'faultyStateName', 'faultyStateName', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 2, 'faultyStateValue', 'faultyStateValue', NULL, true, 'varchar', 0);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 3, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 3, 'id', 'id_safe', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 3, 'id', 'device_id', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 3, 'name', 'device_name', NULL, true, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 3, 'guardDate', 'guardDate', NULL, true, 'date', 0);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 4, 'id', 'id_scheme', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 4, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 4, 'backgroundImageURL', 'backgroundImageURL', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 4, 'parentId', 'parentId', NULL, false, 'int4', 0);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'id', 'element_id', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'gplanId', 'gplanId', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'type', 'shape_type', NULL, true, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'type', 'segment_type', NULL, false, 'varchar', 2);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'type', 'element_type', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'y', 'y', NULL, true, 'varchar', 3);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 5, 'x', 'x', NULL, true, 'varchar', 3);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'id', 'person_id', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, '/fields/field/name', 'f_name', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, '/fields/field/value', 'f_value', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'id', 'card_id', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'code', 'cardCode', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'comment', 'comment', NULL, true, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'type', 'cardTypeId', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'typeName', 'cardTypeName', NULL, true, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'status', 'cardStatusId', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'statusName', 'cardStatusName', NULL, true, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'id', 'plan_id', NULL, true, 'int4', 2);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 6, 'name', 'plan_name', NULL, true, 'varchar', 2);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, '/fields/field/name', 'f_name', NULL, false, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, '/fields/field/value', 'f_value', NULL, false, 'varchar', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'id', 'person_id', NULL, true, 'int4', 1);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'id', 'id_event', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'date', 'date', NULL, true, 'date', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'writeDate', 'writeDate', NULL, true, 'date', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'message', 'message', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'source', 'source', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'alarm', 'alarm', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'deviceId', 'deviceId', NULL, false, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'cardCode', 'cardCode', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'pass', 'pass', NULL, false, 'bool', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'direction', 'direction', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 7, 'videoURL', 'videoURL', NULL, false, 'varchar', 0);

/*
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 8, 'id', 'id_scheme', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 8, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 8, 'backgroundImageURL', 'backgroundImageURL', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 8, 'parentId', 'parentId', NULL, false, 'int4', 0);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'y', 'y', NULL, true, 'varchar', 3);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'x', 'x', NULL, true, 'varchar', 3);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'type', 'element_type', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'id', 'element_id', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'gplanId', 'gplanId', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'type', 'segment_type', NULL, false, 'varchar', 2);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 9, 'type', 'shape_type', NULL, true, 'varchar', 1);

INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'y', 'y', NULL, true, 'varchar', 3);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'x', 'x', NULL, true, 'varchar', 3);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'type', 'element_type', NULL, false, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'id', 'element_id', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'gplanId', 'gplanId', NULL, true, 'int4', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'name', 'name', NULL, true, 'varchar', 0);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'type', 'segment_type', NULL, false, 'varchar', 2);
INSERT INTO xml_paths(id_xml_query_type, xpath, xcode, ref_table, is_mandatory, data_type, xml_level) VALUES ( 10, 'type', 'shape_type', NULL, true, 'varchar', 1);
*/
