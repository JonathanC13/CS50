sqlite3 favorites.db     -- select his db to use for the commands until exit

.schema                 -- To see the commands that were used to create the tables in the db

SELECT * FROM shows     -- view all the data in the table 'shows'

UPDATE shows SET title = "How I Met Your Mother" WHERE title = "How i met your mother"; -- updating

-- shot gun blasting updates
select * from shows where lower(title) like 'brook%';
update shows SET title = 'Brooklyn Nine-Nine' WHERE lower(title) like 'brook%';

select * from shows where lower(title) like 'grey%';
update shows SET title = 'Grey''s Anatomy' WHERE lower(title) like 'grey%';

select * from shows where lower(title) like '%always sunny%';
update shows SET title = 'It''s Always Sunny in Philidelphia' WHERE lower(title) like '%always sunny%';

select * from shows where lower(title) like 'parks and%';
update shows SET title = 'Parks and Recreation' WHERE lower(title) like 'parks and%';

select * from shows where lower(title) like 'the off%';
update shows SET title = 'The Office' WHERE lower(title) like 'the off%';
