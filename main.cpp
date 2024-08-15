#include <iostream>
#include <string>
#include "lib/csv.cpp"


int main() {
    std::string filename = "OrderEventData-2023-04-20_14_00-15_00.csv";
    auto csvData = readCSV(filename);
    
    // Print the data
    int i = 10;
    for (const auto& row : csvData) {
        if (i >= 20) {
            return 0;
        }
        
        for (const auto& cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;

        i += 1;
    }
    
    return 0;
}