#pragma once
#include "CommandTemplate.h"
#include <string>
#include <cstdint>

class Subtract : public CommandTemplate {
private:
    std::string destVar;
    int16_t operand1 = 0;
    int16_t operand2 = 0;
    std::string value0FromProcess = "";
    std::string value1FromProcess = "";
    std::string mode = "";
    bool isCosmetic = false;

public:
    // SUB varA 1 1
    Subtract(const std::string& dest, int16_t op1, int16_t op2);

    // SUB varA 0 varB
    Subtract(const std::string& dest, int16_t op1, const std::string& op2);

    // SUB varA varA 1
    Subtract(const std::string& dest, const std::string& op1, int16_t op2);

    // SUB varA varA varB
    Subtract(const std::string& dest, const std::string& op1, const std::string& op2);
    
    Subtract(bool isCosmeticFlag);

    void execute(Process& process) override;
    InstructionType getType() const override;
};
