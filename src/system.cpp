#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();

    vector<Process> newProcesses = CreateProcesses(pids);
    processes_ = newProcesses;

    return processes_;
}

vector<Process> System::CreateProcesses(vector<int> pids) {
    vector<Process> processes;

    for (int pid : pids) {
        Process process(pid);
        processes.emplace_back(process);
    }

    std::sort(processes.rbegin(), processes.rend());

    return processes;
};

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
