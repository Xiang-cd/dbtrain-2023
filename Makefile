all:
	./build.sh

test:
	./test.sh


l10:
	./build/bin/cli -s < ../dbtrain-lab-test/lab1/test/00_setup.sql

l11:
	./build/bin/cli -s < ../dbtrain-lab-test/lab1/test/10_basic.sql

l12:
	./build/bin/cli -s < ../dbtrain-lab-test/lab1/test/20_error.sql
l13:
	./build/bin/cli -s < ../dbtrain-lab-test/lab1/test/30_long_text.sql

l14:
	./build/bin/cli -s < ../dbtrain-lab-test/lab1/test/40_many_rows.sql

l20:
	./build/bin/cli -s < ../dbtrain-lab-test/lab2/test/00_setup.sql

l21:
	./build/bin/cli -s < ../dbtrain-lab-test/lab2/test/10_abort.sql

l22:
	./build/bin/cli -s < ../dbtrain-lab-test/lab2/test/20_undo.sql

l23:
	./build/bin/cli -s < ../dbtrain-lab-test/lab2/test/30_redo.sql

l24:
	./build/bin/cli -s < ../dbtrain-lab-test/lab2/test/40_checkpoint.sql

l30:
	./build/bin/cli -s < ../dbtrain-lab-test/lab3/test/00_setup.sql

l31:
	./build/bin/cli -s < ../dbtrain-lab-test/lab3/test/10_insert.sql

l32:
	./build/bin/cli -s < ../dbtrain-lab-test/lab3/test/20_update.sql

l33:
	./build/bin/cli -s < ../dbtrain-lab-test/lab3/test/30_delete.sql

l40:
	./build/bin/cli -s < ../dbtrain-lab-test/lab4/test/00_setup.sql
l41:
	./build/bin/cli -s < ../dbtrain-lab-test/lab4/test/10_and_or.sql

l42:
	./build/bin/cli -s < ../dbtrain-lab-test/lab4/test/20_basic_join.sql
l43:
	./build/bin/cli -s < ../dbtrain-lab-test/lab4/test/30_multi_join.sql
l44:
	./build/bin/cli -s < ../dbtrain-lab-test/lab4/test/40_large_join.sql
l50:
	./build/bin/cli -s < ../dbtrain-lab-test/lab5/test/00_setup.sql
l51:
	./build/bin/cli -s < ../dbtrain-lab-test/lab5/test/10_basic_reorder.sql
l52:
	./build/bin/cli -s < ../dbtrain-lab-test/lab5/test/20_reorder_with_filter.sql
l53:
	./build/bin/cli -s < ../dbtrain-lab-test/lab5/test/30_multi_reorder.sql
