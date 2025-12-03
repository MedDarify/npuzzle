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
        // Move Right
        for (int i = left; i <= right; ++i)
        {
            goal[top * N + i] = value++;
        }
        top++;

        // Move Down
        for (int i = top; i <= bottom; ++i)
        {
            goal[i * N + right] = value++;
        }
        right--;

        // Move Left
        if (top <= bottom)
        {
            for (int i = right; i >= left; --i)
            {
                goal[bottom * N + i] = value++;
            }
            bottom--;
        }

        // Move Up
        if (left <= right)
        {
            for (int i = bottom; i >= top; --i)
            {
                goal[i * N + left] = value++;
            }
            left++;
        }
    }

    for (int i = 0; i < N * N; ++i)
    {
        if (goal[i] == N * N)
            goal[i] = 0;
    }

    return goal;
}

// --- 2. ROBUST SOLVABILITY CHECK ---
// We calculate "polarity" for both Start and Goal. They must match.
// Polarity = (Inversions + RowOfZero) % 2 (logic depends on N width)
bool Heuristics::isSolvable(const std::vector<int> &start, const std::vector<int> &goal, int N)
{
    int startInversions = countInversions(start);
    int goalInversions = countInversions(goal);

    if (N % 2 != 0)
    {
        // Odd Width: Only inversion parity matters
        return (startInversions % 2 == goalInversions % 2);
    }
    else
    {
        // Even Width: Parity depends on Inversions + Row of Empty Tile
        int startZeroRow = 0, goalZeroRow = 0;

        // Find row of 0 (from top, 0-indexed)
        for (int i = 0; i < N * N; ++i)
        {
            if (start[i] == 0)
                startZeroRow = i / N;
            if (goal[i] == 0)
                goalZeroRow = i / N;
        }

        // Calculate distance from bottom (or just use row index, as long as consistent)
        // Standard formula uses row from bottom, but comparing (Inv + Row) parity works
        // if we are consistent.

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
            // Note: We are counting inversions based on VALUES.
            // This assumes the goal is 1,2,3... for the purpose of the count.
            // Since we compare StartInversions vs GoalInversions calculated the SAME WAY,
            // the relative parity check works even for Snail goals.
            if (state[i] > state[j])
                inversions++;
        }
    }
    return inversions;
}

// --- 3. HEURISTICS (Updated to use the Goal State map) ---

// Helper to map values to their goal coordinates (O(1) lookup during search)
std::vector<std::pair<int, int>> precomputeGoalPositions(const std::vector<int> &goal, int N)
{
    std::vector<std::pair<int, int>> positions(N * N);
    for (int i = 0; i < N * N; ++i)
    {
        positions[goal[i]] = {i % N, i / N}; // Store x, y for value goal[i]
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