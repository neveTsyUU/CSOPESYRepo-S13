#pragma once
#include "CommandTemplate.h"
#include <string>
#include <optional>
#include <cstdint>

class Subtract : public CommandTemplate {
private:
    std::string destVar;
    int16_t operand1 = 0;
    int16_t operand2 = 0;

public:
    // Full form
    Subtract(const std::string& dest, int16_t op1, int16_t op2);

    // Partial form: only op1
    Subtract(const std::string& dest, int16_t op1);

    // Partial form: only op2
    Subtract(const std::string& dest, std::nullopt_t, int16_t op2);

    // Minimal form: nothing
    explicit Subtract(const std::string& dest);

    void execute(Process& process) override;
    InstructionType getType() const override;
};