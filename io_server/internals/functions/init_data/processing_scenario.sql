insert into processing_scenario (id, name) values (1, '�������� �� ���������');
insert into processing_variant (id, name) values (1, '������� �� ���������');

SELECT pg_catalog.setval('processing_scenario_id_seq', 1, true);
SELECT pg_catalog.setval('processing_variant_id_seq', 1, true);
