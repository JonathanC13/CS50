--select aa.movie_id from stars aa join people bb on bb.id = aa.person_id where bb.name = 'Kevin Bacon'


select distinct(a.name)
from people a
    join stars b on b.person_id = a.id
where b.movie_id IN (select aa.movie_id from stars aa join people bb on bb.id = aa.person_id where bb.name = 'Kevin Bacon' AND bb.birth = 1958)
    AND a.id NOT IN (select aaa.id from people aaa where aaa.name = 'Kevin Bacon' AND aaa.birth = 1958)


