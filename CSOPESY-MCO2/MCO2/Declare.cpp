#include "Declare.h"
#include "Process.h"

Declare::Declare(const std::string& name)
    : varName(name), value(0) {
}

Declare::Declare(const std::string& name, uint16_t val)
    : varName(name), value(val) {
}

Declare::Declare(bool isCosmeticFlag)
{
    isCosmetic = isCosmeticFlag;
}

InstructionType Declare::getType() const {
    return InstructionType::DECLARE;
}


void Declare::execute(Process& process) {
    if (isCosmetic) {
        return;
    }
    process.declareVariable(varName, value);
}