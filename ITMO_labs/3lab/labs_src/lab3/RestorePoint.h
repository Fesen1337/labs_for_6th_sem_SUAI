#pragma once

#include <vector>
#include <string>

#include "Object.h"


class RestorePoint
{
public:
    RestorePoint(){}
    RestorePoint(const std::vector<Object>&obj, const std::string &algo, const std::string &path, int num);
    std::vector<Object> getStorages();
private:
    std::vector<Object>savedObjects;
};
