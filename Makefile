all:
	./build.sh

test:
	./test.sh


l10:
	./build/bin/cli < ../dbtrain-lab-test/lab1/test/00_setup.sql 

l11:
	./build/bin/cli < ../dbtrain-lab-test/lab1/test/10_basic.sql

l12:
	./build/bin/cli < ../dbtrain-lab-test/lab1/test/20_error.sql
l13:
	./build/bin/cli < ../dbtrain-lab-test/lab1/test/30_long_text.sql 

l14:
	./build/bin/cli < ../dbtrain-lab-test/lab1/test/40_many_rows.sql

l20:
	./build/bin/cli < ../dbtrain-lab-test/lab2/test/00_setup.sql

l21:
	./build/bin/cli < ../dbtrain-lab-test/lab2/test/10_abort.sql

l22:
	./build/bin/cli < ../dbtrain-lab-test/lab2/test/20_undo.sql

l23:
	./build/bin/cli < ../dbtrain-lab-test/lab2/test/30_redo.sql

l24:
	./build/bin/cli < ../dbtrain-lab-test/lab2/test/40_checkpoint.sql