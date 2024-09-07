
#include "epexlib/backtesting/basicStrat.hpp"

namespace epexlib {

std::chrono::system_clock::time_point BasicStrategy::getNextTradingTime(
    std::chrono::system_clock::time_point currentTime)
{
    return currentTime + timeDiff;
}

size_t BasicStrategy::getNextTicks()
{
    return tickDiff;
}

std::vector<EventDataRow> BasicStrategy::getOrders(DynamicOrderBook& dynamicOrderBook)
{
    // TODO: implement creation of orders
    return std::vector<EventDataRow>();
}

void BasicStrategy::notifyResult(
    std::vector<EventDataRow>& matchedOrders, DynamicOrderBook& dynamicOrderBook)
{
    // TODO: implement notify
}

}
