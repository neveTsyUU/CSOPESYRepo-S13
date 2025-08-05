#pragma once
#include "CommandTemplate.h"
#include <cstdint>

class Sleep : public CommandTemplate {
private:
    uint8_t sleepDuration;  // Number of CPU ticks to sleep

public:
    explicit Sleep(uint8_t duration);

    void execute(Process& process) override;
    InstructionType getType() const override;
};
