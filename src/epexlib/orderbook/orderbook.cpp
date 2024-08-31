
#include "epexlib/orderbook/orderbook.hpp"
#include <unordered_map>

namespace epexlib {


std::vector<EventDataRow> getLob(const std::vector<EventDataRow>& rows) {
    // Only keep Order Events with highest Revision Number
    std::unordered_map<std::string, int> maxRevMap;
    
    // First pass: find max revision numbers
    for (const auto& row : rows) {
        if (maxRevMap.find(row.orderId) == maxRevMap.end() ||
            maxRevMap[row.orderId] < row.revisionNo) {
            maxRevMap[row.orderId] = row.revisionNo;
        }
    }
    
    // Second pass: filter rows with max revision numbers
    std::vector<EventDataRow> filteredRows;
    std::copy_if(rows.begin(), rows.end(), std::back_inserter(filteredRows),
        [&maxRevMap](const EventDataRow& row) {
            return row.revisionNo == maxRevMap[row.orderId];
        });
    
    // Third pass: filter out deleted rows
    std::vector<EventDataRow> finalRows;
    std::copy_if(filteredRows.begin(), filteredRows.end(), std::back_inserter(finalRows),
        [](const EventDataRow& row) {
            return row.actionCode != ActionCode::D && row.actionCode != ActionCode::H &&
                   row.actionCode != ActionCode::M && row.actionCode != ActionCode::X;
        });
    
    return finalRows;
}

EventData getLobAt(EventData eventData, std::chrono::system_clock::time_point time_stamp)
{
    auto leq_time_stamp = [time_stamp](const EventDataRow& row) {
        return row.transactionTime <= time_stamp;
    };
    eventData = eventData.filter(leq_time_stamp);
    eventData.rows = getLob(eventData.rows);
    return eventData;
}


}
