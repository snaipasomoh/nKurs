CXX=g++
CXXFLAGS=-c -Wall -std=c++14
LDFLAGS=-lpthread

nKurs: main.o
	$(CXX) main.o $(LDFLAGS) -o nKurs

main.o: main.cpp shell.hpp ctpl_stl.h
	$(CXX) $(CXXFLAGS) main.cpp -o main.o

.PHONY: clear

clear:
	$(RM) -r *.o *.gch nKurs
