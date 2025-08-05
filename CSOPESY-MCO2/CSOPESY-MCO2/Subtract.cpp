#include "Subtract.h"
#include "Process.h"
#include <string>

// CONST - CONST
Subtract::Subtract(const std::string& dest, int16_t op1, int16_t op2)
    : destVar(dest), operand1(op1), operand2(op2) {
    mode = "value-value";
}

// CONST - VAR
Subtract::Subtract(const std::string& dest, int16_t op1, const std::string& op2)
    : destVar(dest), operand1(op1), operand2(0), value1FromProcess(op2) {
    mode = "value-var";
}

// VAR - CONST
Subtract::Subtract(const std::string& dest, const std::string& op1, int16_t op2)
    : destVar(dest), operand1(0), operand2(op2), value0FromProcess(op1) {
    mode = "var-value";
}

// VAR - VAR
Subtract::Subtract(const std::string& dest, const std::string& op1, const std::string& op2)
    : destVar(dest), operand1(0), operand2(0), value0FromProcess(op1), value1FromProcess(op2) {
    mode = "var-var";
}

Subtract::Subtract(bool isCosmeticFlag)
{
    isCosmetic = isCosmeticFlag;
}

void Subtract::execute(Process& process) {
    if (isCosmetic) {
        return;
    }

    if (mode == "value-value") {
        process.substract(destVar, mode, "", "", operand1, operand2);
    }
    else if (mode == "value-var") {
        process.substract(destVar, mode, "", value1FromProcess, operand1, 0);
    }
    else if (mode == "var-value") {
        process.substract(destVar, mode, value0FromProcess, "", 0, operand2);
    }
    else if (mode == "var-var") {
        process.substract(destVar, mode, value0FromProcess, value1FromProcess, 0, 0);
    }
    else {
        std::cerr << "Invalid SUBTRACT mode in execute(): " << mode << "\n";
    }
}

InstructionType Subtract::getType() const {
    return InstructionType::SUBTRACT;
}
