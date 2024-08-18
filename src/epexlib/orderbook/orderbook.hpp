#pragma once
#include "epexlib/models/eventData.hpp"

namespace epexlib {

EventData getLOB(EventData eventData, std::chrono::system_clock::time_point time_stamp);

}
