#include "../includes/Heuristics.hpp"
#include <cmath>
#include <algorithm>

int Heuristics::calculateManhattan(const std::vector<int> &state, int N)
{
    int distance = 0;
    for (int i = 0; i < (int)state.size(); ++i)
    {
        int val = state[i];
        if (val == 0)
            continue;

        int targetX = (val - 1) % N;
        int targetY = (val - 1) / N;
        int currentX = i % N;
        int currentY = i / N;

        distance += std::abs(currentX - targetX) + std::abs(currentY - targetY);
    }
    return distance;
}

int Heuristics::calculateHamming(const std::vector<int> &state, int N)
{
    int misplaced = 0;
    for (int i = 0; i < (int)state.size(); ++i)
    {
        int val = state[i];
        if (val == 0)
            continue;
        if (val != i + 1)
        {
            misplaced++;
        }
    }
    return misplaced;
}

int Heuristics::calculateLinearConflict(const std::vector<int> &state, int N)
{
    int h = calculateManhattan(state, N);
    int conflicts = 0;

    // Check Rows
    for (int row = 0; row < N; ++row)
    {
        std::vector<int> rowTiles;
        for (int col = 0; col < N; ++col)
        {
            int val = state[row * N + col];
            if (val != 0 && (val - 1) / N == row)
            {
                rowTiles.push_back(val);
            }
        }
        for (size_t i = 0; i < rowTiles.size(); ++i)
        {
            for (size_t j = i + 1; j < rowTiles.size(); ++j)
            {
                if (rowTiles[i] > rowTiles[j])
                    conflicts++;
            }
        }
    }

    for (int col = 0; col < N; ++col)
    {
        std::vector<int> colTiles;
        for (int row = 0; row < N; ++row)
        {
            int val = state[row * N + col];
            if (val != 0 && (val - 1) % N == col)
            {
                colTiles.push_back(val);
            }
        }
        for (size_t i = 0; i < colTiles.size(); ++i)
        {
            for (size_t j = i + 1; j < colTiles.size(); ++j)
            {
                if (colTiles[i] > colTiles[j])
                    conflicts++;
            }
        }
    }

    return h + (2 * conflicts);
}

int Heuristics::countInversions(const std::vector<int> &state)
{
    int inversions = 0;
    for (size_t i = 0; i < state.size(); ++i)
    {
        if (state[i] == 0)
            continue;
        for (size_t j = i + 1; j < state.size(); ++j)
        {
            if (state[j] == 0)
                continue;
            if (state[i] > state[j])
                inversions++;
        }
    }
    return inversions;
}

bool Heuristics::isSolvable(const std::vector<int> &state, int N)
{
    int inversions = countInversions(state);
    if (N % 2 != 0)
        return (inversions % 2 == 0);
    else
    {
        int zeroIdx = -1;
        for (size_t i = 0; i < state.size(); ++i)
            if (state[i] == 0)
                zeroIdx = i;
        int rowFromBottom = N - (zeroIdx / N);
        if (rowFromBottom % 2 != 0)
            return (inversions % 2 == 0);
        else
            return (inversions % 2 != 0);
    }
}

std::vector<int> Heuristics::generateGoal(int N)
{
    std::vector<int> goal(N * N);
    for (int i = 0; i < N * N - 1; ++i)
        goal[i] = i + 1;
    goal[N * N - 1] = 0;
    return goal;
}