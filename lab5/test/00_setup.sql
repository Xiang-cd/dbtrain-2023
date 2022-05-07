drop database if exists dbtrain_test_lab5;
create database dbtrain_test_lab5;
use dbtrain_test_lab5;

create table persons(id int, name varchar(20), temperature float);

insert into persons values(1, 'a', 36.1), (2, 'b', 36.2), (3, 'c', 36.3);
