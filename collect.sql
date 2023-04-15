drop database if exists dbtrain_test_lab3_plus;
create database dbtrain_test_lab3_plus;
use dbtrain_test_lab3_plus;

create table persons(id int, first_name varchar(20), last_name varchar(1000), temperature float);

begin;
insert into persons values(202, '1', '1', 36.3);
insert into persons values(1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3),
                           (1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3),
                          (1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3);
delete from persons where id =1;
commit;

begin;

checkpoint;

commit;

begin;
insert into persons values(1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3),
                          (1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3),
                          (1, '1', '1', 36.3), (1, '1', '1', 36.3), (1, '1', '1', 36.3);
commit;