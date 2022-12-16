
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { pid_ = pid; }

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
    return static_cast<float>(LinuxParser::ActiveJiffies(Pid()) / 100) / static_cast<float>(LinuxParser::UpTime(Pid()));
}

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() const { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& process) const { return stol(Ram()) < stol(process.Ram()); }
