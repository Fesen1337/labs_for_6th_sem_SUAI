#pragma once
#include <string>
#include <algorithm>

class Object
{
public:
    Object(){}
    Object & withPath(std::string path);
    std::string getName();
    std::string getPath();
private:
    std::string path;
};