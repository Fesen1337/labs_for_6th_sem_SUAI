#pragma once
#include <vector>
#include <string>

#include "Object.h"

class JobObject
{
public:
    void addObject(std::string path);
    void rmObject(std::string path);
    std::vector<Object> getJobObject();
private:
    std::vector<Object>objects;
};