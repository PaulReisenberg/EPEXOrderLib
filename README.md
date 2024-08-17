# EPEXOrderLib

EPEXOrderLib is a C++ library designed to work with EPEX order event data. It provides functionality to process and analyze order book events, including the ability to reconstruct the Limit Order Book at any point in time.

## Features

- Parse EPEX order event data from CSV files
- Reconstruct the Limit Order Book at any specified time
- Efficient data structures for handling large volumes of order events
- Support for various order types and execution restrictions

## Build Commands
To build EPEXOrderLib, run the following command:
```bash
cmake . -B build
cd build
make
```
The binaries will then be built into `build/src`.

## Run Commands
You can run the `main.cpp` executable with:
```bash
src/main <path_to_csv>
```

## Installation

To use EPEXOrderLib, clone this repository and enter the clone directory.
Then run:
```bash
cmake . -B build
cd build
cmake --build .
sudo cmake --install . --prefix <PREFIX>
```
where <PREFIX> is the directory where you want to install EPEXOrderLib.
If you want to install system-wide on Unix systems, use "/usr/local" as the prefix.

## Usage

Here's a basic example of how to use EPEXOrderLib:
(still in development)

```cpp
#include "epexlib/models/eventData.hpp"
#include "epexlib/orderbook/orderbook.hpp"
#include "epexlib/utils/csv.hpp"
#include "epexlib/utils/utils.hpp"
#include <chrono>

int main(int argc, char* argv[])
{
    // Load event data
    epexlib::EventData eventData = epexlib::loadEventDataFromCSV("test.csv", true);
    auto time_stamp = epexlib::stringToTimePoint("2023-04-20T07:32:48");
    // Reconstruct the Limit Order Book
    epexlib::EventData lob = getLOB(eventData, time_stamp);
};

```

## Data Format

EPEXOrderLib expects CSV files with the following structure:

| Column Name         | Example Value                |
|---------------------|------------------------------|
| OrderId             | 14186913885                  |
| InitialId           | 14186903254                  |
| ParentId            | 14186913482                  |
| Side                | BUY                          |
| Product             | XBID_Hour_Power              |
| DeliveryStart       | 2023-04-20T14:00:00Z         |
| DeliveryEnd         | 2023-04-20T15:00:00Z         |
| CreationTime        | 2023-04-20T08:32:45.187Z     |
| DeliveryArea        | DE3                          |
| ExecutionRestriction| NON                          |
| UserdefinedBlock    | N                            |
| LinkedBasketId      | (empty)                      |
| RevisionNo          | 2                            |
| ActionCode          | D                            |
| TransactionTime     | 2023-04-20T08:32:46.882Z     |
| ValidityTime        | (empty)                      |
| Price               | 97.82                        |
| Currency            | EUR                          |
| Quantity            | 1.0                          |
| QuantityUnit        | MAW                          |
| Volume              | 1.0                          |
| VolumeUnit          | MWH                          |

Each row in the CSV file represents a single order event.

## Contributing

Contributions to EPEXOrderLib are welcome! Please feel free to submit a Pull Request.

## Contact

If you have any questions or feedback, please open an issue in this repository.
