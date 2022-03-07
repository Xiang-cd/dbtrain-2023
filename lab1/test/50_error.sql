-- Use non-existing database
use db_not_exists;

use dbtrain_test;

-- Create table with same column names
create table table_error(id int, id int);

-- Drop non-existing table
drop table table_not_exists;

-- Select non-existing table
select * from table_not_exists;

-- Non-existing column in where clause
select * from t_basic where column_not_exists = 1;

-- Non-existing column in select
select id, column_not_exists from t_basic where id = 1;

-- Insert non-existing table
insert into table_not_exists values(1, 2);

-- Insert length mismatch
insert into t_basic values(1, 2);

-- Insert type mismatch
insert into t_basic values(1, 2, 3, 4);

-- Multiple insert type mismatch
insert into t_basic values(9, 9, 'i', 9.9), (10, 10, 10, 10), (11, 11, 11, 11);
