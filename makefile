all: traveling_salesman_problem

traveling_salesman_problem : main.o map.o solution.o tsptabusolver.o
		g++ main.o map.o solution.o tsptabusolver.o -o traveling_salesman_problem

main.o: main.cpp
	g++ -c main.cpp

map.o: map.cpp
	g++ -c map.cpp

solution.o: solution.cpp
	g++ -c solution.cpp

tsptabusolver.o: tsptabusolver.cpp
	g++ -c tsptabusolver.cpp
	
