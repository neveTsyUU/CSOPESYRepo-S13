#include "Declare.h"
#include "Process.h"

Declare::Declare(const std::string& name)
    : varName(name), value(0) {
}

Declare::Declare(const std::string& name, uint16_t val)
    : varName(name), value(val) {
}

InstructionType Declare::getType() const {
    return InstructionType::DECLARE;
}


void Declare::execute(Process& process) {
    uint16_t var = value;
    //process.log("Declared variable " + varName + " = " + std::to_string(value));
}