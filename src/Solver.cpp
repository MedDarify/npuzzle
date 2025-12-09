#include "../includes/Solver.hpp"
#include "../includes/Heuristics.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip> 
#include <fstream> 
#include <iomanip> 
#include <fstream> 

Solver::Solver(int size, std::vector<int> initial, HeuristicFunc hFunc) 
    : N(size), initialBoard(initial), heuristic(hFunc), totalNodesOpened(0), maxNodesInMemory(0) {
    
    goalState = Heuristics::generateSnailGoal(N);
}

void Solver::solve() {
    const std::string RESET  = "\033[0m";
    const std::string BOLD   = "\033[1m";
    const std::string DIM    = "\033[2m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string MAGENTA= "\033[35m";
    const std::string CYAN   = "\033[36m";
    const std::string BG_SUCCESS = "\033[42;1m\033[37m"; 
    const std::string BG_ERROR = "\033[41;1m\033[37m";
    if (!Heuristics::isSolvable({initialBoard, N, goalState})) {
        std::cout << std::endl;
        std::cout << "  " << BG_ERROR << "  FAILED  " << RESET << BOLD << BG_ERROR << "   Puzzle is Unsolvable!" << RESET << std::endl;
        std::cout << std::endl;
        return;
    }

    Node start;
    start.state = initialBoard;
    start.g = 0;
    
    start.h = heuristic({initialBoard, N, goalState}); 
    
    start.f = start.g + start.h;
    
    for(size_t i=0; i<initialBoard.size(); ++i) {
        if(initialBoard[i] == 0) start.zeroPos = i;
    }

    openSet.push(start);
    totalNodesOpened++;

    while (!openSet.empty()) {
        if ((openSet.size() + closedSet.size()) > (size_t)maxNodesInMemory) {
            maxNodesInMemory = openSet.size() + closedSet.size();
        }

        Node current = openSet.top();
        openSet.pop();

        if (current.state == goalState) {
            printSolution(current);
            return;
        }

        closedSet.insert(current.state);

        int z = current.zeroPos;
        int row = z / N;
        int col = z % N;

        struct Move { int dr, dc; std::string name; };
        Move moves[] = {{-1, 0, "UP"}, {1, 0, "DOWN"}, {0, -1, "LEFT"}, {0, 1, "RIGHT"}};

        for (auto& m : moves) {
            int newRow = row + m.dr;
            int newCol = col + m.dc;

            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
                int newZ = newRow * N + newCol;
                std::vector<int> newState = current.state;
                std::swap(newState[z], newState[newZ]);

                if (closedSet.find(newState) != closedSet.end()) continue;

                Node neighbor;

                neighbor.state = newState;
                neighbor.g = current.g + 1;
                neighbor.h = heuristic({newState, N, goalState});
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.zeroPos = newZ;
                neighbor.path = current.path;
                neighbor.path.push_back(m.name);

                openSet.push(neighbor);
                totalNodesOpened++;
            }
        }
    }
}

void Solver::printSolution(const Node& sol) {
    const std::string RESET  = "\033[0m";
    const std::string BOLD   = "\033[1m";
    const std::string DIM    = "\033[2m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string MAGENTA= "\033[35m";
    const std::string CYAN   = "\033[36m";
    const std::string BG_SUCCESS = "\033[42;1m\033[37m"; 

    auto getArrow = [](const std::string& move) -> std::string {
        if (move == "UP")    return "↑";
        if (move == "DOWN")  return "↓";
        if (move == "LEFT")  return "←";
        if (move == "RIGHT") return "→";
        return "";
    };

    std::cout << std::endl;
    std::cout << "  " << BG_SUCCESS << "  SUCCESS  " << RESET << BOLD << GREEN << "  Puzzle Solved!" << RESET << std::endl;
    std::cout << std::endl;
    std::cout << CYAN << "  ┌────────────────────────────────────────────────────────┐" << RESET << std::endl;
    std::cout << CYAN << "  │ " << RESET << BOLD << "Time Complexity " << RESET << "(Nodes Opened)   : " << YELLOW << std::setw(10) << totalNodesOpened << CYAN << "   │" << RESET << std::endl;
    std::cout << CYAN << "  │ " << RESET << BOLD << "Size Complexity " << RESET << "(Max Memory)     : " << YELLOW << std::setw(10) << maxNodesInMemory << CYAN << "   │" << RESET << std::endl;
    std::cout << CYAN << "  │ " << RESET << BOLD << "Path Length     " << RESET << "(Total Moves)    : " << MAGENTA << std::setw(10) << sol.g << CYAN << "   │" << RESET << std::endl;
    std::cout << CYAN << "  └────────────────────────────────────────────────────────┘" << RESET << std::endl;
    std::cout << std::endl;

    std::cout << BOLD << "  Solution Path:" << RESET << std::endl;
    int step = 1;
    int colCounter = 0;
    for (const auto& move : sol.path) {
        if (colCounter == 0) std::cout << "  "; 
        std::cout << DIM << "[" << std::setw(3) << step++ << "] " << RESET << BOLD << getArrow(move) << " " << std::left << std::setw(6) << move << RESET;
        if (++colCounter >= 5) { std::cout << std::endl; colCounter = 0; }
        else std::cout << "   ";
    }
    if (colCounter != 0) 
        std::cout << std::endl; 

    std::string filename = "solution.txt";
    std::ofstream outFile(filename);
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create output file." << std::endl;
        return;
    }

    outFile << "============================================================" << std::endl;
    outFile << "                   N-PUZZLE SOLUTION REPORT                 " << std::endl;
    outFile << "============================================================" << std::endl;
    outFile << "  Algorithm       : A* (A-Star)" << std::endl;
    outFile << "  Time Complexity : " << totalNodesOpened << " nodes opened" << std::endl;
    outFile << "  Size Complexity : " << maxNodesInMemory << " max nodes in memory" << std::endl;
    outFile << "  Total Moves     : " << sol.g << std::endl;
    outFile << "============================================================" << std::endl << std::endl;

    std::vector<int> currentBoard = initialBoard;
    step = 0;

    auto drawLine = [&](int n) {
        outFile << "    "; 
        for(int i=0; i<n; ++i) outFile << "+----";
        outFile << "+" << std::endl;
    };

    auto printBoardToFile = [&](const std::string& moveName, int stepNum) {
        std::string arrow = (moveName == "INITIAL") ? "" : getArrow(moveName);
        
        outFile << "  Step " << std::setw(3) << std::setfill('0') << stepNum << std::setfill(' ') 
                << ": " << moveName << " " << arrow << std::endl;
        
        drawLine(N);
        for (int i = 0; i < N; ++i) {
            outFile << "    "; 
            for (int j = 0; j < N; ++j) {
                int val = currentBoard[i * N + j];
                outFile << "|";
                if (val == 0) outFile << "    "; 
                else outFile << std::setw(3) << val << " ";
            }
            outFile << "|" << std::endl;
            drawLine(N);
        }
        outFile << std::endl;
    };
    printBoardToFile("INITIAL", 0);
    int z = -1;
    for(size_t i=0; i<currentBoard.size(); ++i) if(currentBoard[i] == 0) z = i;

    for (const auto& move : sol.path) {
        step++;
        int row = z / N;
        int col = z % N;
        int targetZ = -1;

        if (move == "UP")         targetZ = (row - 1) * N + col;
        else if (move == "DOWN")  targetZ = (row + 1) * N + col;
        else if (move == "LEFT")  targetZ = row * N + (col - 1);
        else if (move == "RIGHT") targetZ = row * N + (col + 1);

        if (targetZ >= 0 && targetZ < (int)currentBoard.size()) {
            std::swap(currentBoard[z], currentBoard[targetZ]);
            z = targetZ;
        }
        printBoardToFile(move, step);
    }

    outFile.close();
}