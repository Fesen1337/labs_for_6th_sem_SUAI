#include "Repository.h"

void Disk::write(RestorePoint rp, JobObject jo)
{
    int id1 = 0;
    for(auto i : jo.getJobObject())
    {
        std::string comand = "zip ";
        comand += rp.getStorages()[id1].getPath() + " " + i.getPath();
        system(comand.c_str());
        if(id1 + 1 < rp.getStorages().size())
            id1++;
    }
}

Repository::Repository(std::string type)
{
    if(type == "Disk")
        this->fs = new Disk();
    else
        this->fs = new VirtualDisk();
}

void Repository::write(RestorePoint rp, JobObject jo)
{
    this->fs->write(rp, jo);
}