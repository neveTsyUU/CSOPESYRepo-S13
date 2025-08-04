#pragma once
#include "CommandTemplate.h"
#include <string>
#include <optional>

class Print : public CommandTemplate {
public:
    Print();

    Print(const std::string& message);

    Print(const std::string& message, uint16_t varValue);

    void execute(Process& process) override;
    InstructionType getType() const override;

private:
    std::string message;
    std::optional<uint16_t> varValue;
};