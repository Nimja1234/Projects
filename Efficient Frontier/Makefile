CCFLAGS = -std=c++17 -pedantic -Wall -Werror -ggdb3

efficient_frontier: main.o asset.o parser.o solver.o
	g++ $(CCFLAGS) main.o asset.o parser.o -o efficient_frontier

main.o: main.cpp
	g++ $(CCFLAGS) -c main.cpp -o main.o

asset.o: asset.cpp asset.hpp
	g++ $(CCFLAGS) -c asset.cpp -o asset.o

parser.o: parser.cpp
	g++ $(CCFLAGS) -c parser.cpp -o parser.o

solver.o: solver.cpp
	g++ $(CCFLAGS) -c solver.cpp -o solver.o

clean:
	rm -f *.o *~ efficient_frontier
