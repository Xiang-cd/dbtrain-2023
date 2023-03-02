use dbtrain_test_lab3;

-- 3 tables
select b_id1, b_name1, b_score2, b_name2, f_name1 from basic1, basic2, full1 where basic1.b_id1 = basic2.b_id2 and full1.f_id1 = basic1.b_id1;

-- 4 tables
select b_id1, b_name1, b_score2, b_name2, f_name1, f_name2 from basic1, basic2, full1, full2 where basic1.b_id1 = basic2.b_id2 and full1.f_id1 = full2.f_id2 and full2.f_id2 = basic1.b_id1;

-- 5 tables nomatch
select b_id1, b_name1, b_score2, b_name2, f_name1, f_name2, n_name2 from basic1, basic2, full1, full2, nomatch2 where basic1.b_id1 = basic2.b_id2 and full1.f_id1 = basic1.b_id1 and full2.f_id2 = basic2.b_id2 and nomatch2.n_id2 = basic1.b_id1;

-- 5 tables empty
select b_id1, b_name1, b_score2, b_name2, f_name1, f_name2, e_name1 from basic1, basic2, full1, full2, empty1 where basic1.b_id1 = basic2.b_id2 and full1.f_id1 = basic1.b_id1 and full2.f_id2 = basic2.b_id2 and empty1.e_id1 = basic1.b_id1;
