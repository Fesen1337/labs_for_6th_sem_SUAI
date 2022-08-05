#pragma once

#include "string.h"

#include "RestorePoint.h"
#include "JobObject.h"

class FileSystem
{
public:
    virtual void write(RestorePoint rp, JobObject jo) = 0;
};

class VirtualDisk : public FileSystem
{
public:
    VirtualDisk(){}
    void write(RestorePoint rp, JobObject jo){}
};

class Disk : public FileSystem
{
public:
    Disk(){}
    void write(RestorePoint rp, JobObject jo);
};


class Repository
{
public:
    Repository(std::string type);
    std::string getPath();
    void write(RestorePoint rp, JobObject jo);
private:
    std::string path;
    FileSystem* fs;
};
