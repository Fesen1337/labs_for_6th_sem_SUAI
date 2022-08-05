#pragma once

#include <vector>

#include "RestorePoint.h"
#include "JobObject.h"
#include "Repository.h"

class BackupJob
{
public:
    BackupJob(std::string typeFS);
    void addObject(std::string path);
    void rmObject(std::string path);
    void createRestorePoint(std::string algo, std::string path);
    std::vector<Object> getJobObject();
    std::vector<RestorePoint> getRestorePoints();
    JobObject jo;
private:
    Repository* r;
    std::vector<RestorePoint>rPoints;

};