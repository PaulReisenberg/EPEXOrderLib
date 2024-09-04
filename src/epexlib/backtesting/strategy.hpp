

#pragma once
#include "epexlib/models/dynamicOrderBook.hpp"
#include <chrono>

/*
 * This is an abstract strategy class, which specifies which functions
 * a strategy needs to implement.
 * */

namespace epexlib {

class Strategy {
public:
    virtual std::chrono::system_clock::time_point getNextTradingTime() = 0;
    virtual size_t getNextTicks() = 0;

    virtual std::vector<EventDataRow> getOrders(DynamicOrderBook& dynamicOrderBook) = 0;
    virtual void notifyResult(
        std::vector<EventDataRow>& matchedOrders, DynamicOrderBook& dynamicOrderBook) = 0;

    virtual ~Strategy() = default;

    bool useTicks = false;
};

}
