--select createTableCurrDl();
--select * from getUserDls();
--select setCurrentDl(3);

--select * from io_categories;
--insert into io_categories(id_io_type, is_main, name, description) values(3, true, 'Рабочий документ', 'Для тестовых задач');
--select * from attributes;
--54(sent_datetime), 45(message_body), 57(def_width), 42(id_dl_from)

--select * from attrs_categories;
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(43, 54, 'current_timestamp', false, false);
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(43, 45, NULL, true, false);
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(43, 57, 100, false, false);
--insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values(43, 42, NULL, true, false);


select * from io_templates;