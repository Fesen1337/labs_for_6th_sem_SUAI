#include <iostream>

#include "BackupJob.h"
#include "Repository.h"


int main()
{
    BackupJob a("Disk");

    a.addObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/1.txt");
    a.addObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/2.txt");
    a.addObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/3.txt");
    a.addObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/4.txt");
    

    a.createRestorePoint("Split", "~/Desktop/pashaalexeev17-homework-3/My_testing/device/");
    a.createRestorePoint("Single", "~/Desktop/pashaalexeev17-homework-3/My_testing/device/");

    a.rmObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/3.txt");
    a.rmObject("~/Desktop/pashaalexeev17-homework-3/My_testing/test/4.txt");
    

    a.createRestorePoint("Split", "~/Desktop/pashaalexeev17-homework-3/My_testing/device/");
    a.createRestorePoint("Single", "~/Desktop/pashaalexeev17-homework-3/My_testing/device/");

}