#include "BackupJob.h"


BackupJob::BackupJob(std::string typeFS)
{
    r = new Repository(typeFS);
}

void BackupJob::addObject(std::string path)
{
    this->jo.addObject(path);
}

void BackupJob::rmObject(std::string path)
{
    this->jo.rmObject(path);
}

void BackupJob::createRestorePoint(std::string algo, std::string path)
{
    RestorePoint *a = new RestorePoint(this->jo.getJobObject(), algo, path, int(this->rPoints.size() + 1));
    rPoints.push_back(*a);
    r->write(*a, jo);
    delete a;
}

std::vector<Object> BackupJob::getJobObject()
{
    return jo.getJobObject();
}

std::vector<RestorePoint> BackupJob::getRestorePoints()
{
    return rPoints;
}