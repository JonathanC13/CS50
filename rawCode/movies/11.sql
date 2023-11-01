select c.title
from people a
    join stars b on b.person_id = a.id
    join movies c on c.id = b.movie_id
    join ratings d on d.movie_id = c.id
where a.name = 'Chadwick Boseman'
order by d.rating desc
LIMIT 5;