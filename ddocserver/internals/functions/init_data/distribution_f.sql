insert into partition_lows (id, name, description) values (1, '���������� �������������', 'Gaussian distribution');
insert into partition_lows (id, name, description) values (2, '���������������� �������������', 'Exponential distribution');
insert into partition_lows (id, name, description) values (3, '����������� �������������', 'Uniform distribution');

select setval('partition_lows_id_seq', 3, true);
