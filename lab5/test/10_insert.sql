--    Txn #1 | Txn #2 | Txn #3 |
--    --------------------------
--    BEGIN  |        |        |
--    W(X)   |        |        |
--    R(X)   |        |        |
--           | BEGIN  |        |
--           | R(X)   |        |
--    COMMIT |        |        |
--           | R(X)   |        |
--           | COMMIT |        |
--           |        | BEGIN  |
--           |        | R(X)   |
--           |        | COMMIT |

use dbtrain_test_lab5;

declare t1;
  begin;
  insert into persons values (4, 'd', 36.4);
  select * from persons;
  signal t1_2;
  wait t2_1;
  commit;
  signal t1_c;
enddecl t1;

declare t2;
  wait t1_2;
  begin;
  select * from persons;
  signal t2_1;
  wait t1_c;
  select * from persons;
  commit;
  signal t2_c;
enddecl t2;

declare t3;
  wait t2_c;
  begin;
  select * from persons;
  commit;
enddecl t3;

run t1, t2, t3;

--    Txn #4 | Txn #5 | Txn #6 |
--    --------------------------
--    BEGIN  |        |        |
--           | BEGIN  |        |
--           | W(X)   |        |
--    R(X)   |        |        |
--           | R(X)   |        |
--           | COMMIT |        |
--    R(X)   |        |        |
--    COMMIT |        |        |
--           |        | BEGIN  |
--           |        | R(X)   |
--           |        | COMMIT |

declare t4;
  begin;
  signal t4_1;
  wait t5_1;
  select * from persons;
  signal t4_2;
  wait t5_c;
  select * from persons;
  commit;
  signal t4_c;
enddecl t4;

declare t5;
  wait t4_1;
  begin;
  insert into persons values (5, 'e', 36.5);
  signal t5_1;
  wait t4_2;
  select * from persons;
  commit;
  signal t5_c;
enddecl t5;

declare t6;
  wait t4_c;
  begin;
  select * from persons;
  commit;
enddecl t6;

run t4, t5, t6;
