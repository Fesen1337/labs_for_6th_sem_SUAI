#include "JobObject.h"

void JobObject::addObject(std::string path)
{
    int err = 0;
    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i].getPath() == path)
            err++;
    }
    if(err == 0)
    {
        Object* a = new Object();
        this->objects.push_back(a->withPath(path));
        delete a;
    }
}

void JobObject::rmObject(std::string path)
{
    for(int i = 0; i < this->objects.size(); i++)
    {
        if(this->objects[i].getPath() == path)
        {
            this->objects.erase(this->objects.begin() + i);
        }
    }
}

std::vector<Object> JobObject::getJobObject()
{
    return this->objects;
}