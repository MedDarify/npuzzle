#include <iostream>
#include <string>
#include <functional>
#include "../includes/Parser.hpp"
#include "../includes/Solver.hpp"
#include "../includes/Heuristics.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: ./npuzzle <input_file> [manhattan|hamming|linear]" << std::endl;
        return 1;
    }

    try {
        auto result = Parser::parseInput(argv[1]);
        int N = result.first;
        std::vector<int> board = result.second;

        if (board.size() != (size_t)(N * N)) {
            std::cerr << "Error: Board data does not match size " << N << std::endl;
            return 1;
        }

        std::string algoFlag = "manhattan";
        if (argc >= 3) algoFlag = argv[2];

        Solver::HeuristicFunc hFunc;

        if (algoFlag == "hamming") {
            hFunc = Heuristics::calculateHamming;
        } else if (algoFlag == "linear") {
            hFunc = Heuristics::calculateLinearConflict;
        } else {
            hFunc = Heuristics::calculateManhattan;
        }

        Solver solver(N, board, hFunc);
        solver.solve();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}