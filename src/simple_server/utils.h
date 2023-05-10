#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

class Utils {
public:
    static std::vector<std::string> 
        SplitString(std::string str, char token);
    
    static std::string JoinStrings(std::vector<std::string> splits, char token);
};

#endif
