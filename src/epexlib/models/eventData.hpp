
#pragma once

#include "epexlib/models/eventDataRow.hpp"
#include <functional>
#include <vector>

namespace epexlib {

class EventData {
public:
    std::chrono::system_clock::time_point deliveryStart;
    std::chrono::system_clock::time_point deliveryEnd;
    std::string product;
    std::vector<EventDataRow> rows;

    // Constructor with all parameters including data
    EventData(
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        std::string prod,
        std::vector<EventDataRow> rows);

    // Constructor without data parameter
    EventData(
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        std::string prod);

    void addRow(EventDataRow row);

    EventData filter(std::function<bool(const EventDataRow&)> predicate);

    void forEach(std::function<void(const EventDataRow&)> func);

    std::string toString(size_t maxRows = 50) const;

private:
    static std::string timePointToString(const std::chrono::system_clock::time_point& tp);

    static std::string sideToString(Side side);

    static std::string executionRestrictionToString(ExecutionRestriction er);

    static std::string actionCodeToString(ActionCode ac);
};

}
