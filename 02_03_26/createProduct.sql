-- SQLite
create table product (
    p_code text primary key,
    p_descript text not null unique,
    p_indate text,
    p_qoh integer not null default(0),
    p_min integer not null default(0),
    p_price real not null,
    p_discount real,
    v_code integer references vendor(v_code)
)