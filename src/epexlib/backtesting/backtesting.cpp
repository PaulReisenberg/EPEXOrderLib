#include "epexlib/models/eventData.hpp"
#include "epexlib/models/dynamicOrderBook.hpp"
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



    void backtestDay(EventData eventData) {
        // Strategy strategy


        DynamicOrderBook dynamicOrderBook = epexlib::DynamicOrderBook(eventData);
        
        /*

        power_balance = 0
        cash_balance = 0

        while(dynamicOrderBook.isActive()) {

            time = strategy.getNextTradingTime()

            dynamicOrderBook.createNextLobAt(time)
            
            orders = strategy.getOrders(dynamicOrderBook.currentTime);

            matchedOrders = matchOrders(currentLob, orders);

            strategy.notify(matchedOrders, dynamicOrderBook.currentTime);

            updateBalance(matchedOrders);
        }
        */



    }




}

