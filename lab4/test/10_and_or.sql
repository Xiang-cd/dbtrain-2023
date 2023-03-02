use dbtrain_test_lab3;

-- Check basic
select * from basic2 where b_id2 > 1 and b_id2 < 3;
select * from basic2 where b_id2 = 1 or b_id2 > 3;

-- Check empty
select * from basic2 where b_id2 > 2 and b_id2 < 4;
select * from basic2 where b_id2 < 0 or b_id2 > 4;

-- Check redundant condition
select * from basic2 where b_id2 > 1 and b_id2 > 2;
select * from basic2 where b_id2 > 1 or b_id2 > 2;

-- Check different columns
select * from basic2 where b_id2 > 1 and b_score2 > 3.3;
select * from basic2 where b_id2 < 2 or b_name2 = 'basic2_2b';

-- Check multiple conditions
select * from basic2 where b_id2 > 1 and b_id2 < 5 and b_score2 > 2.15;
select * from basic2 where b_id2 < 1 or b_id2 > 3 or b_score2 < 1.15 or b_name2 = 'basic2_1a';
