#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    return static_cast<float>(LinuxParser::ActiveJiffies() * (1.f / LinuxParser::Jiffies()));
}