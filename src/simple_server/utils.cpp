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
