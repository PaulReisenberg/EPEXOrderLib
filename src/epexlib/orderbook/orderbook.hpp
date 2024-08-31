#pragma once
#include "epexlib/models/eventData.hpp"

namespace epexlib {

EventData getLobAt(EventData eventData, std::chrono::system_clock::time_point time_stamp);

std::vector<EventDataRow> getLob(const std::vector<EventDataRow>& rows);

}
