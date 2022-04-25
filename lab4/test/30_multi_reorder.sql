use dbtrain_test_lab4;

analyze;

-- t1, t2, t3, t4
explain select t2.id from t1, t2, t3, t4 where t3.score < 80.0 and t4.id = t3.id and t3.id = t2.id and t2.id = t1.id;

-- t1, t2, t3, t4
explain select t2.id from t1, t2, t3, t4 where t3.score < 80.0 and t4.id = t1.id and t1.id = t2.id and t2.id = t3.id;

-- t1, t2, t3, t4
explain select t2.id from t1, t2, t3, t4 where t3.score < 80.0 and t1.id = t2.id and t1.id = t3.id and t1.id = t4.id;

-- t1, t4, t2, t3
-- explain select t2.id from t1, t2, t3, t4 where t3.score < 80.0 and t4.id = t1.id and t4.id = t2.id and t4.id = t3.id;
