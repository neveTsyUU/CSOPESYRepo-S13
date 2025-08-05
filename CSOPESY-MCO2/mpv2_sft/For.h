#pragma once
#include "CommandTemplate.h"
#include "Config.h"
#include "Print.h"
#include "Declare.h"
#include "Add.h"
#include <vector>
#include <memory>
#include <cstdint>

class For : public CommandTemplate {
public:
    For(uint32_t iterations);
    void createRandomSetOfCommands();
    void execute(Process& process) override;
    InstructionType getType() const override;

private:
    std::vector<std::shared_ptr<CommandTemplate>> miniInstructionList;
    uint32_t repeatCount;


    char getRandomVariableName();
    uint16_t getRandomUint16();
    uint8_t getRandomUint8();
};
