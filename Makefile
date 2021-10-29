CFLAGS=-std=gnu99 -Wall -Wextra -Werror
CC=gcc
LIBS=-lm
FILES=scanner.c scanner.h
PROJ=ifj20

.PHONY: clean 


$(PROJ) : $(FILES)
	$(CC) $(CFLAGS) -o $(PROJ) $(FILES) $(LIBS)


clean:
	rm -f *.o $(PROJ)