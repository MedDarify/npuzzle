#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>
#include <cmath>

class Heuristics {
public:
    static int calculateManhattan(const std::vector<int>& state, int size);
    
    static int calculateHamming(const std::vector<int>& state, int size);
    
    static int calculateLinearConflict(const std::vector<int>& state, int size);

    static bool isSolvable(const std::vector<int>& state, int size);
    static std::vector<int> generateGoal(int size);

private:
    static int countInversions(const std::vector<int>& state);
};

#endif