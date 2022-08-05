#include "Object.h"

Object & Object::withPath(std::string path)
{
    this->path = path;
    return *this;
}


std::string Object::getName()
{
    return this->path.substr(path.find_last_of('/') + 1);
}


std::string Object::getPath()
{
    return this->path;
}