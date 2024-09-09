#include "epexlib/backtesting/backtesting.hpp"
#include "epexlib/marketsim/marketsim.hpp"
#include <sstream>

using std::ostringstream;

namespace epexlib {

void backtestDay(
    EventData eventData, std::chrono::system_clock::time_point starttime, Strategy& strategy)
{
    // Strategy strategy

    DynamicOrderBook dynamicOrderBook = epexlib::DynamicOrderBook(eventData);

    double power_balance = 0;
    double cash_balance = 0;

    auto time = starttime;

    while (dynamicOrderBook.isActive()) {
        time = strategy.getNextTradingTime(time);

        dynamicOrderBook.createNextLobAt(time);

        auto orders = strategy.getOrders(dynamicOrderBook);

        auto matchedOrders = matchOrders(dynamicOrderBook, orders);

        strategy.notifyResult(matchedOrders, dynamicOrderBook);

        updateBalance(power_balance, cash_balance, matchedOrders);
    }
}

}
