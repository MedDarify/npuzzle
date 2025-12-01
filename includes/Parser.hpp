#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
public:
    static std::pair<int, std::vector<int>> parseInput(const std::string &filename);
};

#endif