-- Drop existing database
drop database if exists dbtrain_test;

-- Create new database
create database dbtrain_test;
create database dbtrain_test_2;

-- Check error: create existing database
create database dbtrain_test;

-- Check switch to new database
use dbtrain_test;
use dbtrain_test_2;
use dbtrain_test;

-- Check drop database
drop database dbtrain_test_2;

-- Check create table
create table t_basic(id int, age int, name char(10), score float);
create table t_basic_2(id int, age int, name char(10), score float);

-- Check show tables
show tables;

-- Check desc table
desc t_basic;

-- Check error: create existing table
create table t_basic(id int, age int, name char(10), score float);
show tables;

-- Check drop table
drop table t_basic_2;
show tables;

-- Check error: drop non-existing table
drop table not_exists;

-- Check insert
insert into t_basic values(1, 1, 'a', 1.0);
insert into t_basic values(2, 2, 'b', 2.0);

-- Check multiple insert
insert into t_basic values(3, 3, 'c', 3.0), (4, 4, 'd', 4.0);
insert into t_basic values(5, 5, 'e', 5.5), (6, 6, 'f', 6.6), (7, 7, 'g', 7.7);
