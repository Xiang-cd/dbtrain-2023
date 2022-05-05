drop database if exists dbtrain_test_lab5;
create database dbtrain_test_lab5;
use dbtrain_test_lab5;

create table person(id int, name varchar(20), temperature float);

insert into person values(1, 'a', 36.5);
insert into person values(2, 'b', 37.5);
insert into person values(3, 'c', 38.5);