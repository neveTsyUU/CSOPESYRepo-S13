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

InstructionType Print::getType() const {
    return InstructionType::PRINT;
}

void Print::execute(Process& process) {
    std::string finalMessage;

    if (varValue.has_value()) {
        finalMessage = message + std::to_string(varValue.value());
    }
    else if (message.empty()) {
        finalMessage = "Hello world from " + process.getName() + "!";
    }
    else {
        finalMessage = message;
    }

    process.log("\"" + finalMessage + "\"");
}

