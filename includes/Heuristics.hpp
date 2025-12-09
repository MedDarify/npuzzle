#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>
#include <cmath>

struct HeuristicArgs {
    const std::vector<int>& state;
    int size;
    const std::vector<int>& goalState;
};

class Heuristics {
public:
    static int calculateManhattan(const HeuristicArgs& args);
    static int calculateHamming(const HeuristicArgs& args);
    static int calculateLinearConflict(const HeuristicArgs& args);

    static bool isSolvable(const HeuristicArgs& args);
    static std::vector<int> generateSnailGoal(int size);

private:
    static int countInversions(const std::vector<int>& state);
};

#endif