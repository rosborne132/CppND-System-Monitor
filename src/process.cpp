#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
    float act_time = float(LinuxParser::ActiveJiffies(Pid()))/100.f;
    float up_time = float(LinuxParser::UpTime(Pid()));
    float util = act_time/up_time;

    return float(util);
}

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() const { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& a) const { return stol(Ram()) < stol(a.Ram()); }