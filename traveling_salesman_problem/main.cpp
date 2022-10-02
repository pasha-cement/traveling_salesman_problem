//
//  traveling_salesman_problem
//
//  Created by Павел
//
#include <iostream>
#include "tsptabusolver.hpp"
using namespace std;


int main(int argc, char* argv[]){
    TSPTabuSolver solver3("/Users/pavel/Desktop/TSP-TabuSearch-master/TSP_783_1");
    solver3.solve(1);
    
    return 1;
}
