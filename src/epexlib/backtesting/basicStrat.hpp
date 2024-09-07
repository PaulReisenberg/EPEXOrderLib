
#include "epexlib/backtesting/strategy.hpp"

namespace epexlib {

class BasicStrategy : public Strategy {
public:
    BasicStrategy(std::chrono::system_clock::duration timeDiff)
        : timeDiff(timeDiff)
    {
        tickDiff = 0;
        useTicks = false;
    }

    BasicStrategy(size_t tickDiff)
        : tickDiff(tickDiff)
    {
        // zero time diff
        timeDiff = std::chrono::system_clock::duration::zero();
        useTicks = true;
    }

    virtual std::chrono::system_clock::time_point getNextTradingTime(
        std::chrono::system_clock::time_point currentTime) override;
    virtual size_t getNextTicks() override;
    virtual std::vector<EventDataRow> getOrders(DynamicOrderBook& dynamicOrderBook) override;
    virtual void notifyResult(
        std::vector<EventDataRow>& matchedOrders, DynamicOrderBook& dynamicOrderBook) override;

private:
    // time difference
    std::chrono::system_clock::duration timeDiff;
    // tick difference
    size_t tickDiff;
};

}
