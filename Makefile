CFLAGS=-std=gnu99 -Wall -Wextra -Werror
CC=gcc




.PHONY: clean 

test: scanner_tests.c dyn_string.c
	$(CC) $(CFLAGS) scanner_tests.c dyn_string.c -o scanner_tests
	./scanner_tests
	rm -f scanner_tests

clean:
	rm -f *.o 

