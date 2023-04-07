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
	checkpoint;
	crash; -- 模拟出现崩溃, 需要恢复

	mark_undo_crash; -- 标记为undo时会出现系统奔溃

	use dbtrain_test_lab2; --本次恢复undo 过程中会再次crash

    use dbtrain_test_lab2;
	select * from persons;