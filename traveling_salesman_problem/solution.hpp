//
//  traveling_salesman_problem
//
//  Created by Павел
//
#ifndef SOLUTION_HPP
#define SOLUTION_HPP
#include "map.hpp"

class Solution{
    private:
        double score;
        Map* map;
        int* v;
    public:
        void computeScore();
        void swapSolve(int i, int j);
        Solution(Map* map);
        void initSolution();
        int getV(int key);
        double getScore();
        void set(int key, int value);
        void free();
        void printPath();
};

#endif
