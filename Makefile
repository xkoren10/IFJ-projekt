CFLAGS=-std=gnu99 -Wall -Wextra 
CC=gcc

.PHONY: clean 

make: *.c *.h
	$(CC) $(CFLAGS) *.c *.h -o compiler

test_par: Tests/parser_tests.c dyn_string.c scanner.c symtable.c parser.c parser.h expressions.c stack.c
	$(CC) $(CFLAGS) Tests/parser_tests.c dyn_string.c scanner.c parser.c symtable.c parser.h expressions.c stack.c -o parser_tests
	./parser_tests
	rm -f parser_tests

clean:
	rm -f *.o 