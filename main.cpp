#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Utils

std::chrono::system_clock::time_point stringToTimePoint(const std::string& s)
{
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Invalid time string: " + s);
    }
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    // Parse milliseconds
    char dot;
    int milliseconds;
    if (ss >> dot >> milliseconds) {
        tp += std::chrono::milliseconds(milliseconds);
    }

    return tp;
}

// Data Structures

enum class Side { BUY, SELL };
enum class ExecutionRestriction { NON, AON, FOK, IOC };
enum class ActionCode { D, A, X, P, M, I, C, H };

class EventDataRow {
public:
    std::string orderId;
    std::string initialId;
    std::string parentId;
    Side side;
    std::chrono::system_clock::time_point creationTime;
    std::string deliveryArea;
    ExecutionRestriction executionRestriction;
    std::string userdefinedBlock;
    int revisionNo;
    ActionCode actionCode;
    std::chrono::system_clock::time_point transactionTime;
    float price;
    float volume;

    // Constructor that takes strings (implementation remains the same)
    EventDataRow(
        const std::string& sOrderId,
        const std::string& sInitialId,
        const std::string& sParentId,
        const std::string& sSide,
        const std::string& sCreationTime,
        const std::string& sDeliveryArea,
        const std::string& sExecutionRestriction,
        const std::string& sUserdefinedBlock,
        const std::string& sRevisionNo,
        const std::string& sActionCode,
        const std::string& sTransactionTime,
        const std::string& sPrice,
        const std::string& sVolume)
        : orderId(sOrderId)
        , initialId(sInitialId)
        , parentId(sParentId)
        , side(stringToSide(sSide))
        , creationTime(stringToTimePoint(sCreationTime))
        , deliveryArea(sDeliveryArea)
        , executionRestriction(stringToExecutionRestriction(sExecutionRestriction))
        , userdefinedBlock(sUserdefinedBlock)
        , revisionNo(stoi(sRevisionNo))
        , actionCode(stringToActionCode(sActionCode))
        , transactionTime(stringToTimePoint(sTransactionTime))
        , price(std::stof(sPrice))
        , volume(std::stof(sVolume))
    {
    }

    static EventDataRow rowFromLine(const std::string& line)
    {
        std::vector<std::string> fields;
        std::string field;
        std::istringstream ss(line);

        // Split the line into fields
        while (std::getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Check if we have the correct number of fields
        if (fields.size() != 23) {
            throw std::invalid_argument("Invalid number of fields in line");
        }

        // Parse the fields and create a new EventDataRow
        return EventDataRow(
            fields[1], // OrderId
            fields[2], // InitialId
            fields[3], // ParentId
            fields[4], // Side
            fields[8], // CreationTime
            fields[9], // DeliveryArea
            fields[10], // ExecutionRestriction
            fields[11], // UserdefinedBlock
            fields[13], // RevisionNo
            fields[14], // ActionCode
            fields[15], // TransactionTime
            fields[17], // Price
            fields[21] // Volume
        );
    }

private:
    static Side stringToSide(const std::string& s)
    {
        if (s == "BUY")
            return Side::BUY;
        if (s == "SELL")
            return Side::SELL;
        throw std::invalid_argument("Invalid Side string: " + s);
    }

    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& s)
    {
        std::tm tm = {};
        std::istringstream ss(s);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        if (ss.fail()) {
            throw std::invalid_argument("Invalid time string: " + s);
        }
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        // Parse milliseconds
        char dot;
        int milliseconds;
        if (ss >> dot >> milliseconds) {
            tp += std::chrono::milliseconds(milliseconds);
        }

        return tp;
    }

    static ExecutionRestriction stringToExecutionRestriction(const std::string& s)
    {
        if (s == "NON")
            return ExecutionRestriction::NON;
        if (s == "AON")
            return ExecutionRestriction::AON;
        if (s == "FOK")
            return ExecutionRestriction::FOK;
        if (s == "IOC")
            return ExecutionRestriction::IOC;
        throw std::invalid_argument("Invalid ExecutionRestriction string: " + s);
    }

    static ActionCode stringToActionCode(const std::string& s)
    {
        if (s == "D")
            return ActionCode::D;
        if (s == "A")
            return ActionCode::A;
        if (s == "X")
            return ActionCode::X;
        if (s == "P")
            return ActionCode::P;
        if (s == "M")
            return ActionCode::M;
        if (s == "I")
            return ActionCode::I;
        if (s == "C")
            return ActionCode::C;
        if (s == "H")
            return ActionCode::H;
        throw std::invalid_argument("Invalid ActionCode string: " + s);
    }
};

class EventData {
public:
    std::chrono::system_clock::time_point deliveryStart;
    std::chrono::system_clock::time_point deliveryEnd;
    std::string product;
    std::vector<EventDataRow> rows;

    // Constructor with all parameters including data
    EventData(
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        std::string prod,
        std::vector<EventDataRow> rows)
        : deliveryStart(start)
        , deliveryEnd(end)
        , product(std::move(prod))
        , rows(std::move(rows))
    {
    }

    // Constructor without data parameter
    EventData(
        std::chrono::system_clock::time_point start,
        std::chrono::system_clock::time_point end,
        std::string prod)
        : deliveryStart(start)
        , deliveryEnd(end)
        , product(std::move(prod))
    {
    }

    void addRow(EventDataRow row) { rows.push_back(row); }

    EventData filter(std::function<bool(const EventDataRow&)> predicate)
    {
        std::vector<EventDataRow> newRows;
        std::copy_if(rows.begin(), rows.end(), std::back_inserter(newRows), predicate);
        return EventData(deliveryStart, deliveryEnd, product, newRows);
    }

    void forEach(std::function<void(const EventDataRow&)> func)
    {
        std::for_each(rows.begin(), rows.end(), func);
    }

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "EventData:\n"
            << "Delivery Start: " << timePointToString(deliveryStart) << "\n"
            << "Delivery End: " << timePointToString(deliveryEnd) << "\n"
            << "Product: " << product << "\n"
            << "Number of rows: " << rows.size() << "\n\n";

        // Define column widths
        const int idWidth = 20;
        const int sideWidth = 5;
        const int timeWidth = 25;
        const int areaWidth = 15;
        const int restrictionWidth = 14;
        const int blockWidth = 15;
        const int revWidth = 15;
        const int actionWidth = 10;
        const int priceWidth = 10;
        const int volumeWidth = 10;

        // Print header
        oss << std::left << std::setw(idWidth) << "OrderId" << std::setw(idWidth) << "InitialId"
            << std::setw(idWidth) << "ParentId" << std::setw(sideWidth) << "Side"
            << std::setw(timeWidth) << "CreationTime" << std::setw(areaWidth) << "DeliveryArea"
            << std::setw(restrictionWidth) << "Restriction" << std::setw(blockWidth) << "UserBlock"
            << std::setw(revWidth) << "RevisionNo" << std::setw(actionWidth) << "Action"
            << std::setw(timeWidth) << "TransactionTime" << std::setw(priceWidth) << "Price"
            << std::setw(volumeWidth) << "Volume" << "\n";

        // Print separator
        oss << std::string(
                   idWidth * 3 + sideWidth + timeWidth * 2 + areaWidth + restrictionWidth +
                       blockWidth + revWidth + actionWidth + priceWidth + volumeWidth,
                   '-')
            << "\n";

        // Print rows

        int i = 0;
        for (const auto& row : rows) {
            oss << std::left << std::setw(idWidth) << row.orderId << std::setw(idWidth)
                << row.initialId << std::setw(idWidth) << row.parentId << std::setw(sideWidth)
                << sideToString(row.side) << std::setw(timeWidth)
                << timePointToString(row.creationTime) << std::setw(areaWidth) << row.deliveryArea
                << std::setw(restrictionWidth)
                << executionRestrictionToString(row.executionRestriction) << std::setw(blockWidth)
                << row.userdefinedBlock << std::setw(revWidth) << row.revisionNo
                << std::setw(actionWidth) << actionCodeToString(row.actionCode)
                << std::setw(timeWidth) << timePointToString(row.transactionTime)
                << std::setw(priceWidth) << std::fixed << std::setprecision(2) << row.price
                << std::setw(volumeWidth) << std::fixed << std::setprecision(2) << row.volume
                << "\n";

            if (++i >= 50) {
                break;
            }
        }

        return oss.str();
    }

private:
    static std::string timePointToString(const std::chrono::system_clock::time_point& tp)
    {
        auto ttime = std::chrono::system_clock::to_time_t(tp);
        std::tm* gmt = std::gmtime(&ttime);
        std::stringstream ss;
        ss << std::put_time(gmt, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    static std::string sideToString(Side side)
    {
        switch (side) {
        case Side::BUY:
            return "BUY";
        case Side::SELL:
            return "SELL";
        default:
            return "UNKNOWN";
        }
    }

    static std::string executionRestrictionToString(ExecutionRestriction er)
    {
        switch (er) {
        case ExecutionRestriction::NON:
            return "NON";
        case ExecutionRestriction::AON:
            return "AON";
        case ExecutionRestriction::FOK:
            return "FOK";
        case ExecutionRestriction::IOC:
            return "IOC";
        default:
            return "UNKNOWN";
        }
    }

    static std::string actionCodeToString(ActionCode ac)
    {
        switch (ac) {
        case ActionCode::D:
            return "D";
        case ActionCode::A:
            return "A";
        case ActionCode::X:
            return "X";
        case ActionCode::P:
            return "P";
        case ActionCode::M:
            return "M";
        case ActionCode::I:
            return "I";
        case ActionCode::C:
            return "C";
        case ActionCode::H:
            return "H";
        default:
            return "UNKNOWN";
        }
    }
};

//

EventData loadEventDataFromCSV(const std::string& filename, bool ignoreFirstLine = true)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    if (ignoreFirstLine && !std::getline(file, line)) {
        throw std::runtime_error("File is empty: " + filename);
    }

    std::vector<EventDataRow> rows;

    while (std::getline(file, line)) {
        EventDataRow row = EventDataRow::rowFromLine(line);
        rows.push_back(row);
    }

    std::chrono::system_clock::time_point deliveryStart, deliveryEnd;
    std::string product;

    return EventData(deliveryStart, deliveryEnd, product, rows);
};

EventData getLOB(EventData eventData, std::chrono::system_clock::time_point time_stamp)
{
    auto leq_time_stamp = [time_stamp](const EventDataRow& row) {
        return row.transactionTime <= time_stamp;
    };
    eventData = eventData.filter(leq_time_stamp);

    // Only keep Order Events with highest Revision Number
    std::unordered_map<std::string, int> maxRevMap;
    auto get_max_revNo = [&maxRevMap](const EventDataRow& row) {
        if (maxRevMap.find(row.orderId) == maxRevMap.end() ||
            maxRevMap[row.orderId] < row.revisionNo) {
            maxRevMap[row.orderId] = row.revisionNo;
        }
    };

    eventData.forEach(get_max_revNo);

    auto delete_not_max = [&maxRevMap](const EventDataRow& row) {
        return row.revisionNo == maxRevMap[row.orderId];
    };
    eventData = eventData.filter(delete_not_max);

    auto not_deleted = [](const EventDataRow& row) {
        return row.actionCode != ActionCode::D && row.actionCode != ActionCode::H &&
               row.actionCode != ActionCode::M && row.actionCode != ActionCode::X;
    };

    eventData = eventData.filter(not_deleted);

    return eventData;
}

int main()
{
    std::string filename = "OrderEventData-2023-04-20_14_00-15_00.csv";
    // std::string filename = "example.csv";

    EventData eventData = loadEventDataFromCSV(filename, true);

    std::cout << eventData.toString();

    std::chrono::system_clock::time_point time_stamp = stringToTimePoint("2023-04-20T07:32:48");

    std::cout << "\n\n";

    EventData lob = getLOB(eventData, time_stamp);

    std::cout << lob.toString();

    return 0;
};
