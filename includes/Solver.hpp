#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Node.hpp"
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>

class Solver {
public:
    Solver(int size, std::vector<int> initialBoard, 
           std::function<int(const std::vector<int>&, int)> heuristicFunc);
           
    void solve();

private:
    int N;
    std::vector<int> initialBoard;
    std::vector<int> goalState;
    
    std::function<int(const std::vector<int>&, int)> heuristic; 

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_set<std::vector<int>, VectorHash> closedSet;

    int totalNodesOpened;
    int maxNodesInMemory;

    void printSolution(const Node& solutionNode);
};

#endif