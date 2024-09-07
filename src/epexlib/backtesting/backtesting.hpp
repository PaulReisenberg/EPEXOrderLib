#pragma once

#include "epexlib/backtesting/strategy.hpp"
#include "epexlib/models/eventData.hpp"

namespace epexlib {

void backtestDay(
    EventData eventData, std::chrono::system_clock::time_point starttime, Strategy& strategy);

}
