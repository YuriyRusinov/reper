insert into partition_lows (id, name, description) values (1, 'Нормальное распределение', 'Gaussian distribution');
insert into partition_lows (id, name, description) values (2, 'Экспоненциальное распределение', 'Exponential distribution');
insert into partition_lows (id, name, description) values (3, 'Равномерное распределение', 'Uniform distribution');

select setval('partition_lows_id_seq', 3, true);
