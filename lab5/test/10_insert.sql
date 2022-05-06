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
  insert into person values (4, 'd', 36.4);
  select * from person;
  signal t1_2;
  wait t2_1;
  commit;
  signal t1_c;
enddecl t1;

declare t2;
  wait t1_2;
  begin;
  select * from person;
  signal t2_1;
  wait t1_c;
  select * from person;
  commit;
  signal t2_c;
enddecl t2;

declare t3;
  wait t2_c;
  begin;
  select * from person;
  commit;
enddecl t3;

run t1, t2, t3;
