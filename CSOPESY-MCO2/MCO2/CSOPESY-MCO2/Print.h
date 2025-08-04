#pragma once

#include "CommandTemplate.h"
#include <string>
#include <optional>

class Print : public CommandTemplate {
public:
    Print();

    Print(const std::string& message);

    Print(const std::string& message, uint16_t varValue);

    Print(const std::string& message, const std::string& var);

    Print(bool isCosmeticFlag);

    void execute(Process& process) override;
    InstructionType getType() const override;

private:
    bool isCosmetic = false;
    std::string message;
    std::optional<uint16_t> varValue;
    std::string varFromProcess;
};
