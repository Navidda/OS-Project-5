OBJS = main.o mmu.o memory.o page_table.o
CC = g++
CFLAGS = -c

.PHONY: all clean

all: run

run: $(OBJS)
		$(CC) $(OBJS) -o run

main.o: main.cpp
		$(CC) $(CFLAGS) main.cpp

mmu.o: mmu.cpp mmu.h
		$(CC) $(CFLAGS) mmu.cpp

memory.o: memory.cpp memory.h
		$(CC) $(CFLAGS) memory.cpp

page_table.o: page_table.cpp page_table.h
		$(CC) $(CFLAGS) page_table.cpp

clean:
	rm *.o run
