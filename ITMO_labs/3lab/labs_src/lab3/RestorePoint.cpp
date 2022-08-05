#include "RestorePoint.h"

RestorePoint::RestorePoint(const std::vector<Object>&obj, const std::string &algo, const std::string &path, int num)
{
    if(algo == "Split")
    {
        this->savedObjects = obj;
        for(int i = 0; i < this->savedObjects.size(); i++)
        {
            std::string storage = path + this->savedObjects[i].getName();
            storage += "_" + std::to_string(num) + ".zip";
            this->savedObjects[i].withPath(storage);
        }
    }else{
        Object a;
        std::string storage = path + "Single_Storage_" + std::to_string(num) + ".zip";
        a.withPath(storage);
        this->savedObjects.push_back(a);
    }
    
}

std::vector<Object> RestorePoint::getStorages()
{
    return savedObjects;
}