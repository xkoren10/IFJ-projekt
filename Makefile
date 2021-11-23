CFLAGS=-std=gnu99 -Wall -Wextra -Werror
CC=gcc




.PHONY: clean 

make: dyn_string.c scanner.c
	$(CC) $(CFLAGS) dyn_string.c scanner.c -o scanner

test: Tests/scanner_tests.c dyn_string.c scanner.c
	$(CC) $(CFLAGS) Tests/scanner_tests.c dyn_string.c scanner.c -o scanner_tests
	./scanner_tests
	rm -f scanner_tests

clean:
	rm -f *.o 

