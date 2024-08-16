
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <stdexcept>
#include <string>

inline std::chrono::system_clock::time_point stringToTimePoint(const std::string& s)
{
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Invalid time string: " + s);
    }
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    // Parse milliseconds
    char dot;
    int milliseconds;
    if (ss >> dot >> milliseconds) {
        tp += std::chrono::milliseconds(milliseconds);
    }

    return tp;
}

