#include "../includes/Solver.hpp"
#include "../includes/Heuristics.hpp"
#include <iostream>
#include <algorithm>

Solver::Solver(int size, std::vector<int> initial,
               std::function<int(const std::vector<int> &, int)> hFunc)
    : N(size),
      initialBoard(initial),
      heuristic(hFunc),
      totalNodesOpened(0), maxNodesInMemory(0)
{
    goalState = Heuristics::generateGoal(N);
}

void Solver::solve()
{
    if (!Heuristics::isSolvable(initialBoard, N))
    {
        std::cout << "Unsolvable puzzle" << std::endl;
        return;
    }

    Node start;
    start.state = initialBoard;
    start.g = 0;
    start.h = heuristic(initialBoard, N);
    start.f = start.g + start.h;

    for (size_t i = 0; i < initialBoard.size(); ++i)
    {
        if (initialBoard[i] == 0)
            start.zeroPos = i;
    }

    openSet.push(start);
    totalNodesOpened++;

    while (!openSet.empty())
    {
        if ((openSet.size() + closedSet.size()) > (size_t)maxNodesInMemory)
        {
            maxNodesInMemory = openSet.size() + closedSet.size();
        }

        Node current = openSet.top();
        openSet.pop();

        if (current.state == goalState)
        {
            printSolution(current);
            return;
        }

        closedSet.insert(current.state);

        int z = current.zeroPos;
        int row = z / N;
        int col = z % N;

        struct Move
        {
            int dr, dc;
            std::string name;
        };
        Move moves[] = {{-1, 0, "UP"}, {1, 0, "DOWN"}, {0, -1, "LEFT"}, {0, 1, "RIGHT"}};

        for (auto &m : moves)
        {
            int newRow = row + m.dr;
            int newCol = col + m.dc;

            if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N)
            {
                int newZ = newRow * N + newCol;
                std::vector<int> newState = current.state;
                std::swap(newState[z], newState[newZ]);

                if (closedSet.find(newState) != closedSet.end())
                    continue;

                Node neighbor;
                neighbor.state = newState;
                neighbor.g = current.g + 1;

                neighbor.h = heuristic(newState, N);

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

void Solver::printSolution(const Node &sol)
{
    std::cout << "Complexity in time: " << totalNodesOpened << std::endl;
    std::cout << "Complexity in size: " << maxNodesInMemory << std::endl;
    std::cout << "Number of moves: " << sol.g << std::endl;
    std::cout << "Sequence:" << std::endl;
    for (const auto &move : sol.path)
        std::cout << move << std::endl;
}