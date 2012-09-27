insert into url_types (id, name, win_app, lin_app, extentions) values (1, 'Простой текст', 'notepad', 'elk-textedit', '{1,2,3}');
insert into url_types (id, name, win_app, lin_app, extentions) values (2, 'Документ MS Word (ПК Офис)', 'winword', 'soffice', '{4,5}');
insert into url_types (id, name, win_app, lin_app, extentions) values (3, 'Электронные таблицы (MS Excel)', 'excel', 'soffice', '{6,7}');
insert into url_types (id, name, win_app, lin_app, extentions) values (4, 'Презентации (MS PowerPoint)', 'powerpnt', 'soffice', '{8,9}');
insert into url_types (id, name, win_app, lin_app, extentions) values (5, 'Файлы MS Visio', 'visio', 'soffice', '{10}');
insert into url_types (id, name, win_app, lin_app, extentions) values (6, 'Файлы ГИС "Интеграция"', 'operator', 'operator', '{11,12,13,14,15,16,17,18}');
insert into url_types (id, name, win_app, lin_app, extentions) values (7, 'Файлы ГИС MapInfo', 'mapinfow', '', '{19,20,21,22}');
insert into url_types (id, name, win_app, lin_app, extentions) values (8, 'Файлы ГИС ArcGIS', 'arcgis', '', '{23}');
insert into url_types (id, name, win_app, lin_app, extentions) values (9, 'Простые файлы изображений', 'paint', '', '{24,26,32,33}');
insert into url_types (id, name, win_app, lin_app, extentions) values (10, 'Файлы ERDAS Imagine', '', '', '{24,25,26,27,28,29,30,31,32}');

SELECT pg_catalog.setval('url_types_id_seq', 10, true);
