#pragma once
#include "CommandTemplate.h"
#include <string>
#include <cstdint>

class Read : public CommandTemplate {
private:
    std::string destVar;
    std::string addressStr = "";

public:
    Read(const std::string& dest, const std::string& addressStr);

    void execute(Process& process) override;
    InstructionType getType() const override;
};
