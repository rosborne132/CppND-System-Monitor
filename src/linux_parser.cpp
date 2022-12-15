#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

float LinuxParser::MemoryUtilization() {
    float memTotal = stof(GetValueByKey(kProcDirectory + kMeminfoFilename, "MemTotal:"));
    float memFree = stof(GetValueByKey(kProcDirectory + kMeminfoFilename, "MemFree:"));

    return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
    string uptime, line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }

    return stol(uptime);
 }

long LinuxParser::CalcTotalJiffies(vector<LinuxParser::CPUStates> cpuStates,  vector<string> jiffies) {
    long totalJiffies = 0;

    for (int cpuState : cpuStates) {
        totalJiffies += stol(jiffies[cpuState]);
    };

    return totalJiffies;
}

long LinuxParser::Jiffies() {
    vector<string> jiffies = LinuxParser::CpuUtilization();
    vector<CPUStates> cpuStates = {
        kUser_,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_
    };

    return LinuxParser::CalcTotalJiffies(cpuStates, jiffies);
}

long LinuxParser::ActiveJiffies(int pid) {
    vector<string> jiffies = LinuxParser::GetAllValues(kProcDirectory + to_string(pid) + kStatFilename);

    return stol(jiffies[13] + jiffies[14] + jiffies[15] + jiffies[16]);
}

long LinuxParser::ActiveJiffies() {
    vector<string> jiffies = CpuUtilization();
    vector<CPUStates> cpuStates = {
        kUser_,
        kNice_,
        kSystem_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_
    };

    return LinuxParser::CalcTotalJiffies(cpuStates, jiffies);
}

long LinuxParser::IdleJiffies() {
    vector<string> jiffies = CpuUtilization();
    vector<CPUStates> cpuStates = {
        kIdle_,
        kIOwait_
    };

    return LinuxParser::CalcTotalJiffies(cpuStates, jiffies);
}

vector<string> LinuxParser::CpuUtilization() {
    return LinuxParser::GetAllValues(kProcDirectory + kStatFilename);
}

string LinuxParser::GetValueByKey(string filePath, string searchKey) {
    string line, key, value;
    std::ifstream filestream(filePath);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);

            while (linestream >> key >> value) {
                if (key == searchKey) return value;
            }
        }
    }

    return "0";
}

vector<string> LinuxParser::GetAllValues(std::string filePath) {
    string line, value, key;
    vector<string> values;
    std::ifstream stream(filePath);

    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> key;

        while (linestream >> value) {
            values.push_back(value);
        };
    }

    return values;
}

int LinuxParser::TotalProcesses() {
    return stoi(GetValueByKey(kProcDirectory + kStatFilename, "processes"));
}

int LinuxParser::RunningProcesses() {
    return stoi(GetValueByKey(kProcDirectory + kStatFilename, "procs_running"));
}

string LinuxParser::Command(int pid) {
    string command;
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

    if (stream.is_open()) {
        std::getline(stream, command);
    }

    return command;
}

string LinuxParser::Ram(int pid) {
    return to_string(stol(GetValueByKey(kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:")) / 1000);
}

string LinuxParser::Uid(int pid) {
    return GetValueByKey(kProcDirectory + to_string(pid) + kStatusFilename, "Uid:");
}

string LinuxParser::User(int pid) {
    string line, name, x, uid;
    string userUid = LinuxParser::Uid(pid);
    std::ifstream stream(kPasswordPath);

    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);

            while (linestream >> name >> x >> uid) {
                if (uid == userUid) return name;
            }
        }
    }

    return "UNKNOWN_USER";
}

long LinuxParser::UpTime(int pid) {
    vector<string> processes = LinuxParser::GetAllValues(kProcDirectory + to_string(pid) + kStatFilename);

    return LinuxParser::UpTime() - (stol(processes[21]) / 100);
}
