
#include "epexlib/models/eventDataRow.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace epexlib {

EventDataRow::EventDataRow(
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

EventDataRow EventDataRow::rowFromLine(const std::string& line)
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

Side EventDataRow::stringToSide(const std::string& s)
{
    if (s == "BUY")
        return Side::BUY;
    if (s == "SELL")
        return Side::SELL;
    throw std::invalid_argument("Invalid Side string: " + s);
}

std::chrono::system_clock::time_point EventDataRow::stringToTimePoint(const std::string& s)
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

ExecutionRestriction EventDataRow::stringToExecutionRestriction(const std::string& s)
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

ActionCode EventDataRow::stringToActionCode(const std::string& s)
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

}
