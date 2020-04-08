#include "processor.h"
#include <dirent.h>
#include <unistd.h>
#include <string>

void Processor::void_SetCpuUtilizations(std::vector<std::string>& cpuUtilizations)
{    
    user_ = std::stoi(cpuUtilizations[0]);
    nice_ = stoi(cpuUtilizations[1]);
    system_ = stoi(cpuUtilizations[2]);
    idle_ = stoi(cpuUtilizations[3]);
    iowait_ = stoi(cpuUtilizations[4]);
    irq_ = stoi(cpuUtilizations[5]);
    softirq_ = stoi(cpuUtilizations[6]);
    steal_ = stoi(cpuUtilizations[7]);
    guest_ = stoi(cpuUtilizations[8]);
    guestnice_ = stoi(cpuUtilizations[9]);
}


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{  
     
// Guest time is already accounted in usertime
user_ = user_ - guest_;                     // As you see here, it subtracts guest from user time
nice_ = nice_ - guestnice_;                 // and guest_nice from nice time
idle_ = idle_ + iowait_;                 // ioWait is added in the idleTime
auto systemalltime = system_ + irq_ + softirq_;
auto virtalltime = guest_ + guestnice_;
auto totaltime = user_ + nice_ + systemalltime + idle_ + steal_ + virtalltime;

 return (totaltime- idle_) * 1.0 / totaltime; 
}

