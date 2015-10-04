# makefile

all: memtest

ackerman.o: ackerman.cpp
	g++ -c -g ackerman.cpp

my_allocator.o : my_allocator.cpp
	g++ -c -g my_allocator.cpp

memtest.o : memtest.cpp
	g++ -c -g memtest.cpp

memtest: memtest.o ackerman.o my_allocator.o
	g++ -o memtest memtest.o ackerman.o my_allocator.o
	
clean:
	rm *.o memtest
run:
	./memtest

