insert into time_units (id, name, short_name) values (1, 'seconds', 's');
insert into time_units (id, name, short_name) values (2, 'minutes', 'min');
insert into time_units (id, name, short_name) values (3, 'hours', 'h');
insert into time_units (id, name, short_name) values (4, 'days', 'd');

select setval('time_units_id_seq', 4, true);
