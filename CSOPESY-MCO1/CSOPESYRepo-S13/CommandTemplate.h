#pragma once

#include <cstdint>
#include <string>

class Process;

enum class InstructionType {
    PRINT,
    DECLARE,
    ADD,
    SUBTRACT,
    SLEEP,
    FOR
};

class CommandTemplate {
public:
    virtual ~CommandTemplate() = default;

    // Updated: Accepts reference to Process
    virtual void execute(Process& process) = 0;

    virtual InstructionType getType() const = 0;
};