#include "epexlib/models/eventData.hpp"
#include "epexlib/orderbook/orderbook.hpp"
#include "epexlib/utils/csv.hpp"
#include "epexlib/utils/utils.hpp"
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    std::string filename = "OrderEventData-2023-04-20_14_00-15_00.csv";

    // if existant, read the filename from the first argument
    if (argc > 1) {
        filename = argv[1];
    }

    epexlib::EventData eventData = epexlib::loadEventDataFromCSV(filename, true);

    std::cout << eventData.toString();

    std::chrono::system_clock::time_point time_stamp =
        epexlib::stringToTimePoint("2023-04-20T07:32:48");
    std::cout << "\n\n";

    epexlib::EventData lob = getLOB(eventData, time_stamp);

    std::cout << lob.toString();

    return 0;
};
