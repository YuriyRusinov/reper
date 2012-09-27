insert into url_types (id, name, win_app, lin_app, extentions) values (1, '������� �����', 'notepad', 'elk-textedit', '{1,2,3}');
insert into url_types (id, name, win_app, lin_app, extentions) values (2, '�������� MS Word (�� ����)', 'winword', 'soffice', '{4,5}');
insert into url_types (id, name, win_app, lin_app, extentions) values (3, '����������� ������� (MS Excel)', 'excel', 'soffice', '{6,7}');
insert into url_types (id, name, win_app, lin_app, extentions) values (4, '����������� (MS PowerPoint)', 'powerpnt', 'soffice', '{8,9}');
insert into url_types (id, name, win_app, lin_app, extentions) values (5, '����� MS Visio', 'visio', 'soffice', '{10}');
insert into url_types (id, name, win_app, lin_app, extentions) values (6, '����� ��� "����������"', 'operator', 'operator', '{11,12,13,14,15,16,17,18}');
insert into url_types (id, name, win_app, lin_app, extentions) values (7, '����� ��� MapInfo', 'mapinfow', '', '{19,20,21,22}');
insert into url_types (id, name, win_app, lin_app, extentions) values (8, '����� ��� ArcGIS', 'arcgis', '', '{23}');
insert into url_types (id, name, win_app, lin_app, extentions) values (9, '������� ����� �����������', 'paint', '', '{24,26,32,33}');
insert into url_types (id, name, win_app, lin_app, extentions) values (10, '����� ERDAS Imagine', '', '', '{24,25,26,27,28,29,30,31,32}');

SELECT pg_catalog.setval('url_types_id_seq', 10, true);
