CXX = gcc
CXXFLAGS = -Wall -g
LIB = -lpthread -std=c99

factory: factory.o assemblers.o packers.o
	$(CXX) $(CXXFLAGS) -std=c99 -o factory factory.o assemblers.o packers.o -lpthread


factory.o: factory.c factory.h assemblers.h packers.h 
	$(CXX) $(CXXFLAGS) -std=c99 -c factory.c 

assemblers.o: assemblers.c  factory.h 
	$(CXX) $(CXXFLAGS) -std=c99 -c assemblers.c 

packers.o: packers.c factory.h 
	$(CXX) $(CXXFLAGS) -std=c99 -c packers.c



