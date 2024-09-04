#include "epexlib/models/eventData.hpp"
#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream;

namespace epexlib {

EventData::EventData(
    std::chrono::system_clock::time_point start,
    std::chrono::system_clock::time_point end,
    std::string prod,
    std::vector<EventDataRow> rows)
    : deliveryStart(start)
    , deliveryEnd(end)
    , product(std::move(prod))
    , rows(std::move(rows))
{
    std::sort(
        this->rows.begin(), this->rows.end(), [](const EventDataRow& a, const EventDataRow& b) {
            return a.transactionTime < b.transactionTime;
        });
}

// Constructor without data parameter
EventData::EventData(
    std::chrono::system_clock::time_point start,
    std::chrono::system_clock::time_point end,
    std::string prod)
    : deliveryStart(start)
    , deliveryEnd(end)
    , product(std::move(prod))
{
}

void EventData::addRow(EventDataRow row)
{
    rows.push_back(row);
}

EventData EventData::filter(std::function<bool(const EventDataRow&)> predicate)
{
    std::vector<EventDataRow> newRows;
    std::copy_if(rows.begin(), rows.end(), std::back_inserter(newRows), predicate);
    return EventData(deliveryStart, deliveryEnd, product, newRows);
}

void EventData::forEach(std::function<void(const EventDataRow&)> func)
{
    std::for_each(rows.begin(), rows.end(), func);
}

std::string EventData::toString(size_t maxRows) const
{
    ostringstream oss;
    oss << "EventData:\n"
        << "Delivery Start: " << timePointToString(deliveryStart) << "\n"
        << "Delivery End: " << timePointToString(deliveryEnd) << "\n"
        << "Product: " << product << "\n"
        << "Number of rows: " << rows.size() << "\n\n";

    // Define column widths
    const int idWidth = 20;
    const int sideWidth = 5;
    const int timeWidth = 25;
    const int areaWidth = 15;
    const int restrictionWidth = 14;
    const int blockWidth = 15;
    const int revWidth = 15;
    const int actionWidth = 10;
    const int priceWidth = 10;
    const int volumeWidth = 10;

    // Print header
    oss << std::left << std::setw(idWidth) << "OrderId" << std::setw(idWidth) << "InitialId"
        << std::setw(idWidth) << "ParentId" << std::setw(sideWidth) << "Side"
        << std::setw(timeWidth) << "CreationTime" << std::setw(areaWidth) << "DeliveryArea"
        << std::setw(restrictionWidth) << "Restriction" << std::setw(blockWidth) << "UserBlock"
        << std::setw(revWidth) << "RevisionNo" << std::setw(actionWidth) << "Action"
        << std::setw(timeWidth) << "TransactionTime" << std::setw(priceWidth) << "Price"
        << std::setw(volumeWidth) << "Volume" << "\n";

    // Print separator
    oss << std::string(
               idWidth * 3 + sideWidth + timeWidth * 2 + areaWidth + restrictionWidth + blockWidth +
                   revWidth + actionWidth + priceWidth + volumeWidth,
               '-')
        << "\n";

    // Print rows

    size_t i = 0;
    for (const auto& row : rows) {
        if (i >= maxRows) {
            oss << std::left << std::setw(idWidth) << "..." << std::setw(idWidth) << "..."
                << std::setw(idWidth) << "..." << std::setw(sideWidth) << "..."
                << std::setw(timeWidth) << "..." << std::setw(areaWidth) << "..."
                << std::setw(restrictionWidth) << "..." << std::setw(blockWidth) << "..."
                << std::setw(revWidth) << "..." << std::setw(actionWidth) << "..."
                << std::setw(timeWidth) << "..." << std::setw(priceWidth) << std::fixed << "..."
                << std::setw(volumeWidth) << std::fixed << "..." << "\n";

            oss << "Stopped printing after " << maxRows << " rows.\n";

            break;
        }
        oss << std::left << std::setw(idWidth) << row.orderId << std::setw(idWidth) << row.initialId
            << std::setw(idWidth) << row.parentId << std::setw(sideWidth) << sideToString(row.side)
            << std::setw(timeWidth) << timePointToString(row.creationTime) << std::setw(areaWidth)
            << row.deliveryArea << std::setw(restrictionWidth)
            << executionRestrictionToString(row.executionRestriction) << std::setw(blockWidth)
            << row.userdefinedBlock << std::setw(revWidth) << row.revisionNo
            << std::setw(actionWidth) << actionCodeToString(row.actionCode) << std::setw(timeWidth)
            << timePointToString(row.transactionTime) << std::setw(priceWidth) << std::fixed
            << std::setprecision(2) << row.price << std::setw(volumeWidth) << std::fixed
            << std::setprecision(2) << row.volume << "\n"; 

        ++i;
    }

    return oss.str();
}

std::string EventData::timePointToString(const std::chrono::system_clock::time_point& tp)
{
    auto ttime = std::chrono::system_clock::to_time_t(tp);
    std::tm* gmt = std::gmtime(&ttime);
    std::stringstream ss;
    ss << std::put_time(gmt, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string EventData::sideToString(Side side)
{
    switch (side) {
    case Side::BUY:
        return "BUY";
    case Side::SELL:
        return "SELL";
    default:
        return "UNKNOWN";
    }
}

std::string EventData::executionRestrictionToString(ExecutionRestriction er)
{
    switch (er) {
    case ExecutionRestriction::NON:
        return "NON";
    case ExecutionRestriction::AON:
        return "AON";
    case ExecutionRestriction::FOK:
        return "FOK";
    case ExecutionRestriction::IOC:
        return "IOC";
    default:
        return "UNKNOWN";
    }
}

std::string EventData::actionCodeToString(ActionCode ac)
{
    switch (ac) {
    case ActionCode::D:
        return "D";
    case ActionCode::A:
        return "A";
    case ActionCode::X:
        return "X";
    case ActionCode::P:
        return "P";
    case ActionCode::M:
        return "M";
    case ActionCode::I:
        return "I";
    case ActionCode::C:
        return "C";
    case ActionCode::H:
        return "H";
    default:
        return "UNKNOWN";
    }
}

}
