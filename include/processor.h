#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>


class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void void_SetCpuUtilizations(std::vector<std::string>& cpuUtilizations);

  // TODO: Declare any necessary private members
 private:
    int user_ {0};
    int nice_ {0};
    int system_ {0};
    int idle_ {0};
    int iowait_ {0};
    int irq_ {0};
    int softirq_ {0};
    int steal_ {0};
    int guest_ {0};
    int guestnice_ {0};
};

#endif