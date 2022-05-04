drop database if exists dbtrain_test_lab5;
create database dbtrain_test_lab5;
use dbtrain_test_lab5;

create table person(id int, name varchar(20), temperature float);

insert into person values(1, 'a', 36.5), (2, 'b', 36.6);
