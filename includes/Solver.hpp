#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Node.hpp"
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>

class Solver {
public:
    // Constructor updated to match new heuristic signature
    Solver(int size, std::vector<int> initialBoard, 
           std::function<int(const std::vector<int>&, int, const std::vector<int>&)> heuristicFunc);
           
    void solve();

private:
    int N;
    std::vector<int> initialBoard;
    std::vector<int> goalState;
    
    // Heuristic now accepts (State, Size, GoalState)
    std::function<int(const std::vector<int>&, int, const std::vector<int>&)> heuristic; 

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_set<std::vector<int>, VectorHash> closedSet;

    int totalNodesOpened;
    int maxNodesInMemory;

    void printSolution(const Node& solutionNode);
};

#endif