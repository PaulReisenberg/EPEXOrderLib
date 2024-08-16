
#include "orderbook/orderbook.hpp"
#include <unordered_map>

EventData getLOB(EventData eventData, std::chrono::system_clock::time_point time_stamp)
{
    auto leq_time_stamp = [time_stamp](const EventDataRow& row) {
        return row.transactionTime <= time_stamp;
    };
    eventData = eventData.filter(leq_time_stamp);

    // Only keep Order Events with highest Revision Number
    std::unordered_map<std::string, int> maxRevMap;
    auto get_max_revNo = [&maxRevMap](const EventDataRow& row) {
        if (maxRevMap.find(row.orderId) == maxRevMap.end() ||
            maxRevMap[row.orderId] < row.revisionNo) {
            maxRevMap[row.orderId] = row.revisionNo;
        }
    };

    eventData.forEach(get_max_revNo);

    auto delete_not_max = [&maxRevMap](const EventDataRow& row) {
        return row.revisionNo == maxRevMap[row.orderId];
    };
    eventData = eventData.filter(delete_not_max);

    auto not_deleted = [](const EventDataRow& row) {
        return row.actionCode != ActionCode::D && row.actionCode != ActionCode::H &&
               row.actionCode != ActionCode::M && row.actionCode != ActionCode::X;
    };

    eventData = eventData.filter(not_deleted);

    return eventData;
}

