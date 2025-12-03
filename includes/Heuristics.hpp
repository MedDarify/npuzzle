#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>
#include <cmath>

class Heuristics
{
public:
    static int calculateManhattan(
        const std::vector<int> &state,
        int size,
        const std::vector<int> &goalState);
    static int calculateHamming(
        const std::vector<int> &state,
        int size,
        const std::vector<int> &goalState);
    static int calculateLinearConflict(
        const std::vector<int> &state,
        int size,
        const std::vector<int> &goalState);

    static bool isSolvable(
        const std::vector<int> &initialState,
        const std::vector<int> &goalState,
        int size);
    static std::vector<int> generateSnailGoal(int size);

private:
    static int countInversions(const std::vector<int> &state);
};

#endif