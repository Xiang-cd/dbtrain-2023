use dbtrain_test;

-- Check database
select * from t_basic;
insert into t_basic values(8, 8, 'h', 8.8);
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
