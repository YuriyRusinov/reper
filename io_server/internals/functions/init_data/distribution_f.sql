insert into partition_lows (id, name, description) values (1, 'Ќормальное распределение', 'Gaussian distribution');
insert into partition_lows (id, name, description) values (2, 'Ёкспоненциальное распределение', 'Exponential distribution');
insert into partition_lows (id, name, description) values (3, '–авномерное распределение', 'Uniform distribution');

select setval('partition_lows_id_seq', 3, true);
