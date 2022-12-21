#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;
using std::stringstream;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// Work based on project feedback.
string Format::ElapsedTime(long seconds) {
    // Create time conversion
    long hours, minutes;
    hours = seconds / 3600;
    seconds = seconds % 3600;
    minutes = seconds / 60;
    seconds = seconds % 60;

    string sec = to_string(seconds);
    string mins = to_string(minutes);
    string hrs = to_string(hours);

    // Format time strings with values
    sec.insert(0, 2 - sec.length(), '0');
    mins.insert(0, 2 - mins.length(), '0');
    hrs.insert(0, 2 - hrs.length(), '0');

    return hrs + ":" + mins + ":" + sec;
}