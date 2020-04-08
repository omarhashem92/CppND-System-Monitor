#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Done
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  
  string line;
  string key;
  string value;
  float memoryTotal;
  float memoryFree;
  bool memoryTotalFlag{false};
  bool memoryFreeFlag{false};
  float usedMemory;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memoryTotal =  stof(value);
          memoryTotalFlag = true;
        }
        if(key == "MemFree"){
          memoryFree =  stof(value);
          memoryFreeFlag = true;
        }
        if(memoryTotalFlag == true && memoryFreeFlag == true){
          break;
        }
      }
      if(memoryTotalFlag == true && memoryFreeFlag == true){
          break;
      }
    }
  usedMemory = ((memoryTotal - memoryFree) / memoryTotal);
  return usedMemory;
  }
  else{
    std::clog<<"couldn't open memory info file"<<std::endl;
    
  }
  //std::cout<<"Memory utilization is "<<usedMemory<<std::endl;
  return 0;
}


//Done but not sure about Value here.
// TODO: Read and return the system uptime
  long LinuxParser::UpTime() { 
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> value;
  }
  return stoi(value); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  string value;
  string line;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			stat_list.push_back(value);
      }
    }
  long utime{std::stol(stat_list[13])};
  long stime{std::stol(stat_list[14])};
  long cutime{std::stol(stat_list[15])};
  long cstime{std::stol(stat_list[16])};
  long starttime{std::stol(stat_list[21])};
  
  return (utime + stime + cutime + cstime + starttime);
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  vector<string> jiff;
  string line;
  string value;
  string restValues;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    while (linestream >> restValues) {
      jiff.push_back(restValues);
    }
  }
  return jiff;
 }

float LinuxParser::ProcessUtilization(int pid)
{  
  string fileName = kProcDirectory + std::to_string(pid) + kStatFilename;
  string value;
  string line;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			stat_list.push_back(value);
      }
    }
  long utime{std::stol(stat_list[13])};
  long stime{std::stol(stat_list[14])};
  long cutime{std::stol(stat_list[15])};
  long cstime{std::stol(stat_list[16])};
  long starttime{std::stol(stat_list[21])};

  long total_time = utime + stime;
  total_time = total_time + cutime + cstime;
  auto system_up_time = UpTime();
  float seconds = system_up_time - (starttime / sysconf(_SC_CLK_TCK));
  float cpu_usage =  ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  //std::cout<<"Hi, CPU usage is "<<cpu_usage<<std::endl;
   return cpu_usage;
}


// TODO: Read and return the total number of processes
//Done
int LinuxParser::TotalProcesses() {

  string line;
  string key ="";
  int value {0}; 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") 
          return value; 
    }
  }
  else{
    std::clog<<"couldn't find total processes"<<std::endl;
    
  }
  return 0;
 }

// TODO: Read and return the number of running processes
//Done
int LinuxParser::RunningProcesses() { 
  
  string line;
  string key;
  int value {0}; 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") 
          return value; 
    }
  }
  else{
    std::clog<<"couldn't find running processes"<<std::endl;
    
  }
  return 0;
}  
  
//Done
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;
  //string path = kProcDirectory+"pid"; //currently not working
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line; 
  }
   else{
    std::clog<<"couldn't find command"<<std::endl;
    
  }
  return 0;
 }

//DONE
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line;
  string key;
  string value;
  string dummy;
  int ram;
  //string path = kProcDirectory+"pid";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value >> dummy ) {
        	if (key == "VmSize:") { 
              	ram = std::stoi(value); 
              	ram = ram / 1000;
              	return std::to_string(ram);
               }
        	}
      	}
    }
    else{
      std::clog<<"couldn't open status file"<<std::endl;
      
    }
    return "Ram Error";
 }

//Done
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string line;
  string key;
  string value; 
  //string path = kProcDirectory+ "pid";
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") 
          return value; 
    }
  }
  else{
    std::clog<<"couldn't find Uid"<<std::endl;
    
  }
  return "Not found";
 }


//DONE
// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string line;
  string keyUser;
  string uidValue;
  string dummy;
  std::ifstream filestream(kPasswordPath);
  string uid_ = LinuxParser::Uid(pid);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
    	while (linestream >> keyUser >> dummy >> uidValue) {
          if (uid_ == uidValue){
                return keyUser;
          }
      }
    }
  }
  else{
    std::clog<<"couldn't find User"<<std::endl;
    
  }
  return 0;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  string value;
  string line;
  //string path = kProcDirectory+"pid";
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			stat_list.push_back(value);
      }
    }
  auto uptime = std::stol(stat_list[21])/sysconf(_SC_CLK_TCK);
  return uptime;
 }

