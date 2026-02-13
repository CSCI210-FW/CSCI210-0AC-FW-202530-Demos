-- SQLite
select *
from product
    right join vendor on product.v_code = vendor.v_code
order by v_code