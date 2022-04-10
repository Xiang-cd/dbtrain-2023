use dbtrain_test_lab3;

select b_id1, b_name1, b_score2, b_name2 from basic1, basic2 where basic1.b_id1 = basic2.b_id2;

select f_id1, f_name1, f_name2 from full1, full2 where full1.f_id1 = full2.f_id2;

select n_id1, n_name1, n_name2 from nomatch1, nomatch2 where nomatch1.n_id1 = nomatch2.n_id2;

select e_id1, e_name1, e_name2 from empty1, empty2 where empty1.e_id1 = empty2.e_id2;

select b_id1, b_name1, b_score2, b_name2 from basic1, basic2 where basic1.b_id1 = 1 and basic1.b_id1 = basic2.b_id2;
