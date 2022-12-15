#include <thread>
#include <chrono>

#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    long allJiffies = LinuxParser::Jiffies();
    long activeJiffies = LinuxParser::ActiveJiffies();

    // Pause for one second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    long allCurrentJiffies = LinuxParser::Jiffies();
    long currentActiveJiffies = LinuxParser::ActiveJiffies();

    // Create deltas
    long allJiffiesDelta = allCurrentJiffies - allJiffies;
    long activeJiffiesDelta = currentActiveJiffies - activeJiffies;

    return static_cast<float>(activeJiffiesDelta / allJiffiesDelta);
}