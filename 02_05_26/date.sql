-- SQLite
select (julianday('now') - julianday(inv_date)) / 365
from invoice