use dbtrain_test;

create table join_table_1(id int, name varchar(10));

create table join_table_2(id int, num int);

create table join_table_3(id int, num2 int);

create table join_table_empty_1(id int, num_empty_1 int);

create table join_table_empty_2(id int, num_empty_2 int);

insert into join_table_1 values(1, 'a'), (2, 'b'), (3, 'c');

insert into join_table_2 values(1, 2), (2, 15);

insert into join_table_3 values(1, 120), (3, 800);

select * from join_table_1, join_table_2 where join_table_1.id = join_table_2.id;

select name from join_table_1, join_table_2 where join_table_1.id = join_table_2.id;

select join_table_2.num from join_table_1, join_table_2 where join_table_1.id = join_table_2.id;

select * from join_table_1, join_table_2, join_table_3 where join_table_1.id = join_table_2.id and join_table_1.id = join_table_3.id;

select * from join_table_1, join_table_2 where join_table_1.id = join_table_2.id and join_table_2.num > 13;

select * from join_table_1, join_table_2 where join_table_1.id = join_table_2.id and join_table_1.name = 'a' and join_table_2.num > 13;

select * from join_table_1, join_table_empty_1 where join_table_1.id = join_table_empty_1.id;

select * from join_table_empty_1, join_table_1 where join_table_empty_1.id = join_table_1.id;

select * from join_table_empty_1, join_table_empty_2 where join_table_empty_1.id = join_table_empty_2.id;

select * from join_table_1, join_table_2, join_table_empty_1 where join_table_1.id = join_table_2.id and join_table_1.id = join_table_empty_1.id;
