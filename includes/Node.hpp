#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <string>
#include <functional>

struct Node
{
    std::vector<int> state;
    int g;       // Cost from start
    int h;       // Heuristic cost
    int f;       // Total cost (g + h)
    int zeroPos; // Index of the empty tile (0)

    std::vector<std::string> path;

    bool operator>(const Node &other) const
    {
        return f > other.f;
    }
};

struct VectorHash
{
    size_t operator()(const std::vector<int> &v) const
    {
        size_t seed = 0;
        for (int i : v)
        {
            seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

#endif