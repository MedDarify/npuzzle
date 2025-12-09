#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Node.hpp"
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>
#include "../includes/Heuristics.hpp"

class Solver {
public:
    using HeuristicFunc = std::function<int(const HeuristicArgs&)>;

    Solver(int size, std::vector<int> initialBoard, HeuristicFunc hFunc);
           
    void solve();
    void printSolution(const Node& sol);

private:
    int N;
    std::vector<int> initialBoard;
    std::vector<int> goalState;
    
    HeuristicFunc heuristic;

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::unordered_set<std::vector<int>, VectorHash> closedSet;

    int totalNodesOpened;
    int maxNodesInMemory;
};

#endif