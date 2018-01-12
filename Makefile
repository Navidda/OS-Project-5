all: clean mmu.o main.o
	g++ -o run main.o mmu.o

main.o:
	g++ -c main.cpp

mmu.o:
	g++ -c mmu.cpp

clean:
	rm *.o run