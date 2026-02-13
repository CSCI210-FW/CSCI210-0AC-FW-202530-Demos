-- SQLite
SELECT actor.*
FROM actor
    JOIN cast as c ON actor.id = c.actor_id
    JOIN movie ON c.movie_id = movie.id
WHERE movie.year >= 1850
    and movie.year < 1900
INTERSECT
SELECT actor.*
FROM actor
    JOIN cast as c ON actor.id = c.actor_id
    JOIN movie ON c.movie_id = movie.id
WHERE movie.year >= 1900
    and movie.year < 1950