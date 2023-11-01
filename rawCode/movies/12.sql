select a.title
from movies a
where a.id IN (select aa.movie_id from stars aa join people bb on bb.id = aa.person_id where bb.name = 'Bradley Cooper')
    AND a.id IN (select aa.movie_id from stars aa join people bb on bb.id = aa.person_id where bb.name = 'Jennifer Lawrence');
