#pragma once
#include "CommandTemplate.h"
#include <string>
#include <optional>
#include <cstdint>

class Add : public CommandTemplate {
private:
    std::string destVar;
    int16_t operand1 = 0;
    int16_t operand2 = 0;
    std::string value0FromProcess = "";
    std::string value1FromProcess = "";
    std::string mode = "";
    bool isCosmetic = false;

public:
    // ADD varA varA varB
    Add(const std::string& dest, const std::string& op1, const std::string& op2);

    // ADD varA 0 varB
    Add(const std::string& dest, int16_t op1, const std::string& op2);

    // ADD varA varA 1
    Add(const std::string& dest, const std::string& op1, int16_t op2);

    // ADD varA 1 1
    Add(const std::string& dest, int16_t op1, int16_t op2);

    Add(bool isCosmeticFlag);

    void execute(Process& process) override;
    InstructionType getType() const override;
};
