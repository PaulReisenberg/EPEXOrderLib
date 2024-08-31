#include "epexlib/models/eventData.hpp"
#include "epexlib/models/dynamicOrderBook.hpp"
#include "epexlib/orderbook/orderbook.hpp"
#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace epexlib {


DynamicOrderBook::DynamicOrderBook(epexlib::EventData eventData):
    
    deliveryStart(eventData.deliveryStart),
    deliveryEnd(eventData.deliveryEnd),
    product(eventData.product),
    lastVwap(0.0 / 0.0),
    currentTime(),
    currentLob(EventData(deliveryStart, deliveryEnd, product)),
    remainingEventData(eventData)
{
}


void DynamicOrderBook::createNextLobByTicks(int ticks)
{

    // get the next n rows from the remaining LOB
    int rowsToProcess = std::min(ticks, static_cast<int>(remainingEventData.rows.size()));
    std::vector<EventDataRow> nextRows(remainingEventData.rows.begin(), 
                                       remainingEventData.rows.begin() + rowsToProcess);

    // Remove the processed rows from remainingEventData
    remainingEventData.rows.erase(remainingEventData.rows.begin(), 
                                  remainingEventData.rows.begin() + rowsToProcess);


    // merge the rows with the current LOB
    currentLob.rows.insert(currentLob.rows.end(), nextRows.begin(), nextRows.end());


    // get the LOB
    currentLob.rows = epexlib::getLob(currentLob.rows);
}


size_t getIndexUpToTimestamp(const std::vector<EventDataRow>& remainingEventData,
                             const std::chrono::system_clock::time_point& time_stamp)
{
    // Find the first row that exceeds the time_stamp
    auto it = std::find_if(remainingEventData.begin(), remainingEventData.end(),
        [&time_stamp](const EventDataRow& row) {
            return row.transactionTime >= time_stamp;
        });

    // Return the index of the last element to be included
    // If all elements are to be included, this will return remainingEventData.size()
    return std::distance(remainingEventData.begin(), it);
}


void DynamicOrderBook::createNextLobAt(std::chrono::system_clock::time_point time_stamp)
{

    // get the next n rows from the remaining LOB
    size_t rowsToProcess = getIndexUpToTimestamp(remainingEventData.rows, time_stamp);

    std::vector<EventDataRow> nextRows(remainingEventData.rows.begin(), 
                                       remainingEventData.rows.begin() + rowsToProcess);

    // Remove the processed rows from remainingEventData
    remainingEventData.rows.erase(remainingEventData.rows.begin(), 
                                  remainingEventData.rows.begin() + rowsToProcess);


    // merge the rows with the current LOB
    currentLob.rows.insert(currentLob.rows.end(), nextRows.begin(), nextRows.end());


    // get the LOB
    currentLob.rows = epexlib::getLob(currentLob.rows);
}

EventData DynamicOrderBook::getCurrentLob() {
    return currentLob;
}


double DynamicOrderBook::getLastVwap()
{
    return lastVwap;
}




}