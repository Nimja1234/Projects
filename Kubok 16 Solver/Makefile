CCFLAGS = -std=c++17 -pedantic -Wall -Werror -ggdb3

kubok: main.o parser.o kuboksolver.o
	g++ $(CCFLAGS) main.o parser.o kuboksolver.o -o kubok

main.o: main.cpp
	g++ $(CCFLAGS) -c main.cpp -o main.o

parser.o: parser.cpp
	g++ $(CCFLAGS) -c parser.cpp -o parser.o

kuboksolver.o: kuboksolver.cpp
	g++ $(CCFLAGS) -c kuboksolver.cpp -o kuboksolver.o

clean:
	rm -f *.o *~ kubok