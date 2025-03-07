EXE = allocate
SOURCES = main.c file_reader.c queue.c round_robin_first_fit.c round_robin_infinite.c memory.c round_robin_paged.c page.c round_robin_virtual.c page_virtual.c
CFLAGS = -Wall
CC = gcc

$(EXE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) -lm -g

clean:
	rm -f $(EXE) *.o