use dbtrain_test;

create table table_error(id int, id int);

drop table table_not_exists;

select * from table_not_exists;

select * from t_basic where column_not_exists = 1;

select id, column_not_exists from t_basic where id = 1;

insert into table_not_exists values(1, 2);

insert into t_basic values(1, 2, 3);

insert into t_basic values(1, 2, 3, 4);

insert into t_basic values(9, 9, 'i', 9.9), (10, 10, 10, 10), (11, 11, 11, 11);
