
#include "utils/csv.hpp"
#include <fstream>

EventData loadEventDataFromCSV(const std::string& filename, bool ignoreFirstLine)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    if (ignoreFirstLine && !std::getline(file, line)) {
        throw std::runtime_error("File is empty: " + filename);
    }

    std::vector<EventDataRow> rows;

    while (std::getline(file, line)) {
        EventDataRow row = EventDataRow::rowFromLine(line);
        rows.push_back(row);
    }

    std::chrono::system_clock::time_point deliveryStart, deliveryEnd;
    std::string product;

    return EventData(deliveryStart, deliveryEnd, product, rows);
};
