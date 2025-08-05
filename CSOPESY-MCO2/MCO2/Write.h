#pragma once
#include "CommandTemplate.h"
#include <string>
#include <cstdint>

class Write : public CommandTemplate {
private:
    std::string addressStr = "";
    int16_t constantValue = 0;
    std::string valueFromProcess = "";
    std::string mode = "";

public:
    // WRITE 0x500 12
    Write(const std::string& addressStr, int16_t value);

    // WRITE 0x500 varA
    Write(const std::string& addressStr, const std::string& varName);

    void execute(Process& process) override;
    InstructionType getType() const override;
};

