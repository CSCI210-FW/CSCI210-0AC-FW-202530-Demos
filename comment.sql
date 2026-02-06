-- SQLite
select inv_date
from invoice
where -- inv_date > '2018-01-24' 
    -- and inv_date < '2018-10-10'
    inv_date between '2018-01-24' and '2018-10-10'