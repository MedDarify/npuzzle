#include "../includes/Parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

std::pair<int, std::vector<int>> Parser::parseInput(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Could not open file.");
    }

    std::string line;
    std::vector<int> board;
    int N = 0;
    bool sizeFound = false;

    while (getline(file, line))
    {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos)
            line = line.substr(0, commentPos);

        if (line.empty())
            continue;

        std::stringstream ss(line);
        if (!sizeFound)
        {
            if (ss >> N)
                sizeFound = true;
        }
        else
        {
            int val;
            while (ss >> val)
                board.push_back(val);
        }
    }
    return {N, board};
}