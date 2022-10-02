//
//  traveling_salesman_problem
//
//  Created by Павел
//
#ifndef TSP_TABU_SOLVER_HPP
#define TSP_TABU_SOLVER_HPP

#include "map.hpp"
#include "solution.hpp"
class TSPTabuSolver{
    public:
        TSPTabuSolver(string filePath);
        void solve(int);
        Solution* getBestNearbySolution(int);
        void resetTabuList();
    private:
        double bestSolverScore;
        int** tabu_list;
        int** tabu_f_list;
        Map* map;
        Solution* s;
};

#endif
