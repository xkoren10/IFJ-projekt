<<<<<<< HEAD
CFLAGS=-std=gnu99 -Wall -Wextra 
CC=gcc

.PHONY: clean 


make: ifj21_compiler.c dyn_string.c scanner.c parser.c symtable.c  expressions.c code_gen.c stack.c
	$(CC) $(CFLAGS) ifj21_compiler.c dyn_string.c scanner.c parser.c  symtable.c expressions.c code_gen.c stack.c -o ifj21_compiler


clean:
	rm -f *.o 
=======
CFLAGS=-std=gnu99 -Wall -Wextra 
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

>>>>>>> main
