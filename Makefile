all: mmu.o main.o
	g++ -o run main.o mmu.o

main.o: mmu.o main.cpp
	g++ -c main.cpp

mmu.o: mmu.cpp mmu.h
	g++ -c mmu.cpp

clean:
	rm *.o run