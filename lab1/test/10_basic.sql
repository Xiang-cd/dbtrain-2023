use dbtrain_test;

-- Check database
select * from t_basic;
insert into t_basic values(8, 8, 'h', 8.8);
select * from t_basic;

-- Check normal selection
select * from t_basic where id = 1;
select * from t_basic where age < 3;
select * from t_basic where name = 'g';
select * from t_basic where score > 5.4;

-- Check empty selection
select * from t_basic where id > 10;

-- Check normal update
update t_basic set name = 'cc' where name = 'c';
select * from t_basic;

-- Check multiple update
update t_basic set name = 'cc' where id < 3;
select * from t_basic;

-- Check multiple update and multiple set
update t_basic set score = 66.6, name = 'ccc' where id < 4;
select * from t_basic;

-- Check empty update
update t_basic set age = 8 where id > 10;
select * from t_basic;

-- Check normal deletion
delete from t_basic where id = 8;
select * from t_basic;
delete from t_basic where id = 3;
select * from t_basic;

-- Check range deletion
delete from t_basic where id < 3;
select * from t_basic;

-- Check empty deletion
delete from t_basic where id > 10;
select * from t_basic;
