CFLAGS=-std=gnu99 -Wall -Wextra 
CC=gcc

.PHONY: clean 


make: ifj21_compiler.c dyn_string.c scanner.c parser.c symtable.c  expressions.c code_gen.c stack.c
	$(CC) $(CFLAGS) ifj21_compiler.c dyn_string.c scanner.c parser.c  symtable.c expressions.c code_gen.c stack.c -o ifj21_compiler


clean:
	rm -f *.o 