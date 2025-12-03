#include "../includes/Heuristics.hpp"
#include <cmath>
#include <algorithm>
#include <map>
#include <iostream>

std::vector<int> Heuristics::generateSnailGoal(int N)
{
    std::vector<int> goal(N * N);
    int value = 1;
    int top = 0, bottom = N - 1, left = 0, right = N - 1;

    while (top <= bottom && left <= right)
    {
        for (int i = left; i <= right; ++i)
            goal[top * N + i] = value++;
        top++;
        for (int i = top; i <= bottom; ++i)
            goal[i * N + right] = value++;
        right--;
        if (top <= bottom)
        {
            for (int i = right; i >= left; --i)
                goal[bottom * N + i] = value++;
            bottom--;
        }
        if (left <= right)
        {
            for (int i = bottom; i >= top; --i)
                goal[i * N + left] = value++;
            left++;
        }
    }
    for (int i = 0; i < N * N; ++i)
        if (goal[i] == N * N)
            goal[i] = 0;

    return goal;
}

bool Heuristics::isSolvable(const std::vector<int> &start, const std::vector<int> &goal, int N)
{
    int startInversions = countInversions(start);
    int goalInversions = countInversions(goal);

    if (N % 2 != 0)
        return (startInversions % 2 == goalInversions % 2);
    else
    {
        int startZeroRow = 0, goalZeroRow = 0;
        for (int i = 0; i < N * N; ++i)
        {
            if (start[i] == 0)
                startZeroRow = i / N;
            if (goal[i] == 0)
                goalZeroRow = i / N;
        }

        int startPolarity = (startInversions + startZeroRow) % 2;
        int goalPolarity = (goalInversions + goalZeroRow) % 2;
        return (startPolarity == goalPolarity);
    }
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

std::vector<std::pair<int, int>> precomputeGoalPositions(const std::vector<int> &goal, int N)
{
    std::vector<std::pair<int, int>> positions(N * N);
    for (int i = 0; i < N * N; ++i)
    {
        positions[goal[i]] = {i % N, i / N};
    }
    return positions;
}

int Heuristics::calculateManhattan(const std::vector<int> &state, int N, const std::vector<int> &goal)
{
    static std::vector<std::pair<int, int>> goalPos;
    if (goalPos.empty())
        goalPos = precomputeGoalPositions(goal, N);

    int distance = 0;
    for (int i = 0; i < (int)state.size(); ++i)
    {
        int val = state[i];
        if (val == 0)
            continue;

        int targetX = goalPos[val].first;
        int targetY = goalPos[val].second;
        int currentX = i % N;
        int currentY = i / N;

        distance += std::abs(currentX - targetX) + std::abs(currentY - targetY);
    }
    return distance;
}

int Heuristics::calculateHamming(
    const std::vector<int> &state,
    int size,
    const std::vector<int> &goal)
{
    int misplaced = 0;
    std::cout << size << std::endl;
    for (int i = 0; i < (int)state.size(); ++i)
    {
        if (state[i] == 0)
            continue;
        if (state[i] != goal[i])
            misplaced++;
    }
    return misplaced;
}

int Heuristics::calculateLinearConflict(
    const std::vector<int> &state,
    int N,
    const std::vector<int> &goal)
{
    int h = calculateManhattan(state, N, goal);
    int conflicts = 0;

    static std::vector<std::pair<int, int>> goalPos;
    if (goalPos.empty())
        goalPos = precomputeGoalPositions(goal, N);

    for (int row = 0; row < N; ++row)
    {
        std::vector<int> rowTiles;
        for (int col = 0; col < N; ++col)
        {
            int val = state[row * N + col];
            if (val != 0 && goalPos[val].second == row)
            {
                rowTiles.push_back(val);
            }
        }
        for (size_t i = 0; i < rowTiles.size(); ++i)
        {
            for (size_t j = i + 1; j < rowTiles.size(); ++j)
            {
                if (goalPos[rowTiles[i]].first > goalPos[rowTiles[j]].first)
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
            if (val != 0 && goalPos[val].first == col)
            {
                colTiles.push_back(val);
            }
        }
        for (size_t i = 0; i < colTiles.size(); ++i)
        {
            for (size_t j = i + 1; j < colTiles.size(); ++j)
            {
                if (goalPos[colTiles[i]].second > goalPos[colTiles[j]].second)
                    conflicts++;
            }
        }
    }

    return h + (2 * conflicts);
}