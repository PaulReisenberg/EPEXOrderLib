
#include "epexlib/utils/csv.hpp"
#include "epexlib/utils/utils.hpp"
#include <fstream>

namespace epexlib {

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

    std::string firstLine = "";

    while (std::getline(file, line)) {
        if (firstLine == "") {
            firstLine = line;
        }
        EventDataRow row = EventDataRow::rowFromLine(line);
        rows.push_back(row);
    }

    // read deliveryTimes from first line
    size_t pos = findNthChar(firstLine, ',', 6);
    size_t pos2 = firstLine.find(',', pos + 1);
    size_t pos3 = firstLine.find(',', pos2 + 1);

    std::string substr1 = firstLine.substr(pos + 1, pos2 - pos - 1);
    std::string substr2 = firstLine.substr(pos2 + 1, pos3 - pos2 - 1);

    std::chrono::system_clock::time_point deliveryStart = stringToTimePoint(substr1);
    std::chrono::system_clock::time_point deliveryEnd = stringToTimePoint(substr2);

    std::string product = productFromDelivery(deliveryStart, deliveryEnd);

    return EventData(deliveryStart, deliveryEnd, product, rows);
};

}
