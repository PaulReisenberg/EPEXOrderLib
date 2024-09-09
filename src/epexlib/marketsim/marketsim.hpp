
#pragma once

#include "epexlib/models/dynamicOrderBook.hpp"
#include "epexlib/models/eventDataRow.hpp"
#include <vector>

namespace epexlib {

std::vector<EventDataRow> matchOrders(DynamicOrderBook& dob, std::vector<EventDataRow> orders);

void updateBalance(
    double& powerBalance, double& cashBalance, std::vector<EventDataRow> matchedOrders);
}
