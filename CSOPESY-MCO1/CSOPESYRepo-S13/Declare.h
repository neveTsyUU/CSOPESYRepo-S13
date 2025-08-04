#pragma once
#pragma once
#include "CommandTemplate.h"
#include <string>

class Declare : public CommandTemplate {
private:
    std::string varName;
    uint16_t value;

public:
    explicit Declare(const std::string& name);
    Declare(const std::string& name, uint16_t val);

    void execute(Process& process) override;
    InstructionType getType() const override;
};