PROGRAMS = data

CFLAGS = -g -std=c99 -Wall -Werror

programs: $(PROGRAMS)

data: tuple.o generic.o table.o hash_table.o main.o parser.o
	$(CC) -o $@ $^ -lm
	
clean:
	-rm $(PROGRAMS) *.o