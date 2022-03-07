use dbtrain_test;

select * from t_basic where id > 0;

select id, name from t_basic where id < 3;

select t_basic.score, t_basic.id from t_basic where id < 3 and id > 1;

select * from t_basic where t_basic.id < 3 and t_basic.id > 1 and t_basic.name = 'a';

select * from t_basic where id = 1 and name = 'a';

select id, name, score from t_basic where id = 2 or name = 'c' or score = 2;

select id, score from t_basic where score > 7;

select id from t_basic where score = 'a';

select * from t_basic where name < 4;
