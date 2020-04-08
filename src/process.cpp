
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
// TODO: Return this process's ID


int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO (DONE): Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_;} 




// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    float obj1Cpu = cpu_;
    float obj2Cpu = a.cpu_;
    if(obj1Cpu < obj2Cpu)
    {
        return true;
    }
    else{
        return false;
    }   
} 