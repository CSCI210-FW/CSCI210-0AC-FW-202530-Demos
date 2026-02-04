-- SQLite
create table customer (
    cus_code integer primary key,
    cus_lname text not null,
    cus_fname text not null,
    cus_initial text,
    cus_areacode text not null default('260'),
    cus_phone text not null,
    cus_balance real default(0)
)