#include <iostream>
#include "tsptabusolver.hpp"
using namespace std;


int main(int argc, char* argv[]){

    TSPTabuSolver solver2("tsp0.txt");
    solver2.solve(6);
    return 0;
}
