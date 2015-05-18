CREATE TABLE sales(year int, month int, qty int);
INSERT INTO sales VALUES(2007, 1, 1000);
INSERT INTO sales VALUES(2007, 2, 1500);
INSERT INTO sales VALUES(2007, 7, 500);
INSERT INTO sales VALUES(2007, 11, 1500);
INSERT INTO sales VALUES(2007, 12, 2000);
INSERT INTO sales VALUES(2008, 1, 1000);
INSERT INTO sales VALUES(2009, 5, 2500);
INSERT INTO sales VALUES(2009, 9, 800);
 
SELECT * FROM sales;
 year | month | qty  
------+-------+------
 2007 |     1 | 1000
 2007 |     2 | 1500
 2007 |     7 |  500
 2007 |    11 | 1500
 2007 |    12 | 2000
 2008 |     1 | 1000
 2009 |     5 | 2500
 2009 |     9 |  800
(8 rows)
 
SELECT * FROM crosstab(
  $$ SELECT year, month, qty FROM sales ORDER BY 1 $$,
  $$ SELECT m FROM generate_series(1,12) m $$
) AS (
  year int, "Jan" int, "Feb" int, "Mar" int, "Apr" int, "May" int, "Jun" int, "Jul" int, "Aug" int, "Sep" int, "Oct" int, "Nov" int, "Dec" int
);
