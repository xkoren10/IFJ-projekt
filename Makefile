CFLAGS=-std=gnu99 -Wall -Wextra 
CC=gcc

.PHONY: clean 

make: *.c *.h
	$(CC) $(CFLAGS) *.c *.h -o compiler

test_par: Tests/parser_tests.c dyn_string.c scanner.c symtable.c parser.c parser.h expressions.c code_gen.c stack.c
	$(CC) $(CFLAGS) Tests/parser_tests.c dyn_string.c scanner.c code_gen.c  parser.c symtable.c parser.h expressions.c stack.c -o parser_tests
	./parser_tests
	rm -f parser_tests

<<<<<<< HEAD
test_exp: Tests/expressions_tests.c dyn_string.c scanner.c symtable.c expressions.c stack.c code_gen.c
	$(CC) $(CFLAGS) Tests/expressions_tests.c dyn_string.c scanner.c  symtable.c expressions.c stack.c code_gen.c -o expressions_tests
=======
test_exp: Tests/expressions_tests.c dyn_string.c scanner.c symtable.c  expressions.c stack.c code_gen.c
	$(CC) $(CFLAGS) Tests/expressions_tests.c code_gen.c dyn_string.c scanner.c  symtable.c  expressions.c stack.c -o expressions_tests
>>>>>>> 35d25fe1cc60d4413c5e4558ac8ea3a5a5be9726
	./expressions_tests
	rm -f expressions_tests

test_scan: Tests/scanner_tests.c dyn_string.c scanner.c
	$(CC) $(CFLAGS) Tests/scanner_tests.c dyn_string.c scanner.c -o scanner_tests
	./scanner_tests
	rm -f scanner_tests


clean:
	rm -f *.o 