use dbtrain_test_lab4;

analyze;

-- t2, t1, t3
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id and t2.score < 10.0;

-- t2, t1, t3
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id and t2.score > 80.0 and t2.score < 90.0;

-- t2, t1, t3
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id and t2.score < 30.0 and t2.temp < 36.0;

-- t3, t2, t1
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id and t2.score < 30.0 and t3.score > 95.0 and t1.score < 200;

-- t2, t3, t1
explain select t2.id from t2, t3, t1 where t2.id = t3.id and t2.id = t1.id and t2.score < 30.0 and t3.score > 95.0;
