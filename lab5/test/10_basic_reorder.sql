use dbtrain_test_lab4;

analyze;

-- t1, t2, t3 or t2, t1, t3
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id;

-- t1, t2, t3 or t2, t1, t3
explain select t2.id from t2, t3, t1 where t2.id = t1.id and t2.id = t3.id;