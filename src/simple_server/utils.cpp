#include "utils.h"

#include <sstream>
#include <iostream>

std::vector<std::string>
Utils::SplitString(std::string str, char token)
{
    std::vector<std::string> result;

    std::istringstream is(str);
    std::string s;

    while(getline(is, s, token))
    {
        result.push_back(s);
    }

    return result;
}

std::string Utils::JoinStrings(std::vector<std::string> splits, char token)
{
    std::string result;

    for (int i = 0; i < splits.size() - 1; i++) 
    {
        result += splits[i] + token;
    }

    result += splits[splits.size()-1];

    return result;
};
