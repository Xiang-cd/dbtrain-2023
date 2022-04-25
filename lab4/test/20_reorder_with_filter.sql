use dbtrain_test_lab4;

analyze;

-- t1, t3, t2 or t3, t1, t2
explain select t2.id from t1, t2, t3 where t3.id = t1.id and t3.id = t2.id and t3.score < 10.0;

-- t1, t3, t2 or t3, t1, t2
explain select t2.id from t1, t2, t3 where t3.id = t1.id and t3.id = t2.id and t3.score > 80.0 and t3.score < 90.0;

-- t1, t3, t2 or t3, t1, t2
explain select t2.id from t1, t2, t3 where t3.id = t1.id and t3.id = t2.id and t3.score < 30.0 and t3.temp < 36.0;

-- t2, t3, t1 or t3, t2, t1
explain select t2.id from t1, t2, t3 where t3.id = t1.id and t3.id = t2.id and t2.score < 10.0 and t3.score > 97.0 and t1.score < 200.0;
