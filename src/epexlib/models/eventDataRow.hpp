
#pragma once
#include <chrono>
#include <string>

namespace epexlib {

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
        const std::string& sVolume);

    static EventDataRow rowFromLine(const std::string& line);

private:
    static Side stringToSide(const std::string& s);

    static std::chrono::system_clock::time_point stringToTimePoint(const std::string& s);

    static ExecutionRestriction stringToExecutionRestriction(const std::string& s);

    static ActionCode stringToActionCode(const std::string& s);
};

}
