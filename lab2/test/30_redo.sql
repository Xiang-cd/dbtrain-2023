use dbtrain_test_lab2;

begin;

insert into persons values(201, '12345678901234567890', '09876543210987654321', 36.3);

delete from persons where id = 1;

update persons set temperature = 37.3 where id = 100;

commit;

crash;

recover;

select * from persons;
