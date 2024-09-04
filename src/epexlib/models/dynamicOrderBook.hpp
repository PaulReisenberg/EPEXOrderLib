#pragma once

#include "epexlib/models/eventData.hpp"

namespace epexlib {

class DynamicOrderBook {
public:
    std::chrono::system_clock::time_point deliveryStart;
    std::chrono::system_clock::time_point deliveryEnd;
    std::string product;
    
    double lastVwap;
    std::chrono::system_clock::time_point currentTime;
    epexlib::EventData currentLob;
    epexlib::EventData remainingEventData;
    

    DynamicOrderBook(epexlib::EventData eventData);


    void createNextLobAt(std::chrono::system_clock::time_point time_stamp);

    void createNextLobByTicks(int ticks);

    EventData getCurrentLob();

    double getLastVwap();

    bool isActive();

    };
}
