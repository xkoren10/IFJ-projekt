CFLAGS=-std=gnu99 -Wall -Wextra 
CC=gcc

.PHONY: clean 

make: dyn_string.c scanner.c parser.c expressions.c
	$(CC) $(CFLAGS) dyn_string.c scanner.c parser.c expressions.c -o compiler

test_sc: Tests/scanner_tests.c dyn_string.c scanner.c
	$(CC) $(CFLAGS) Tests/scanner_tests.c dyn_string.c scanner.c -o scanner_tests
	./scanner_tests
	rm -f scanner_tests

test_par: Tests/parser_tests.c dyn_string.c scanner.c
	$(CC) $(CFLAGS) Tests/parser_tests.c dyn_string.c scanner.c parser.c expressions.c -o parser_tests
	./parser_tests
	rm -f parser_tests

clean:
	rm -f *.o 