
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace epexlib {

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

inline size_t findNthChar(const std::string& s, char c, size_t n)
{
    size_t pos = 0;
    for (size_t i = 0; i < n; ++i) {
        pos = s.find(c, pos + 1);
    }
    return pos;
}

inline std::string productFromDelivery(
    std::chrono::system_clock::time_point deliveryStart,
    std::chrono::system_clock::time_point deliveryEnd)
{
    // calculate difference in time in minutes
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(deliveryEnd - deliveryStart);
    std::string product = std::to_string(duration.count()) + "m";
    return product;
}

}
