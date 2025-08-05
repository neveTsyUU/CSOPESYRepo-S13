#include "Print.h"
#include "Process.h" // for getName(), getCurrentFormattedTime(), getAssignedCPUId(), log()

Print::Print()
    : message(""), varValue(std::nullopt) {
}

Print::Print(const std::string& message)
    : message(message), varValue(std::nullopt) {
}

Print::Print(const std::string& message, uint16_t varValue)
    : message(message), varValue(varValue) {
}

Print::Print(const std::string& message, const std::string& var)
    : message(message), varFromProcess(var) {
}

Print::Print(bool isCosmeticFlag)
{
    isCosmetic = isCosmeticFlag;
}

InstructionType Print::getType() const {
    return InstructionType::PRINT;
}

void Print::execute(Process& process) {
    if (isCosmetic) {
        std::string message = "This is a message from " + process.getName() + ".";
        process.log("\"" + message + "\"");
        return;
    }

    if (process.symbolTable.find(varFromProcess) == process.symbolTable.end()) {
        // Do nothing if variable is not found
        return;
    }

    std::string finalMessage = message + std::to_string(process.symbolTable[varFromProcess]);
    process.log("\"" + finalMessage + "\"");
}

//void Print::execute(Process& process) {
//
//
//
//    //std::string finalMessage;
//
//    //if (varValue.has_value()) {
//    //    finalMessage = message + std::to_string(varValue.value());
//    //}
//    //else if (message.empty()) {
//    //    finalMessage = "Hello world from " + process.getName() + "!";
//    //}
//    //else {
//    //    finalMessage = message;
//    //}
//
//    /*process.log("\"" + finalMessage + "\"");*/
//}
