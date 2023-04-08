drop database if exists dbtrain_test_lab2;

create database dbtrain_test_lab2;
use dbtrain_test_lab2;

create table persons(id int, first_name varchar(20), last_name varchar(20), temperature float);

begin;
insert into persons values (1, '12345678901234567890', '09876543210987654321', 36.3), 
(2, '12345678901234567890', '09876543210987654321', 36.3),
(3, '12345678901234567890', '09876543210987654321', 36.3);
commit;
flush;

begin;
	insert into persons values(203, '12345678901234567890', '09876543210987654321', 36.3);
	insert into persons values(204, '12345678901234567890', '09876543210987654321', 36.3);
commit;
checkpoint;

begin;
insert into persons values(205, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values(206, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values(205, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values(206, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values(206, '12345678901234567890', '09876543210987654321', 36.3);
insert into persons values(206, '12345678901234567890', '09876543210987654321', 36.3);
commit;