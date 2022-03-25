use dbtrain_test_lab2;

begin;

insert into persons values(202, '12345678901234567890', '09876543210987654321', 36.3);

delete from persons where id = 201;

update persons set temperature = 36.8 where id = 100;

commit;

begin;

delete from persons where id > 190;

checkpoint;

insert into persons values(203, '12345678901234567890', '09876543210987654321', 36.3);

update persons set temperature = 37.3 where id > 180;

crash;

use dbtrain_test_lab2;

select * from persons;
