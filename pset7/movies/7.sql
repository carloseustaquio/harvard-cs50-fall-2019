SELECT title, rating
FROM movies
JOIN ratings ON movies.id = movie_id
WHERE year = 2010
AND rating != ""
ORDER BY rating DESC, title;