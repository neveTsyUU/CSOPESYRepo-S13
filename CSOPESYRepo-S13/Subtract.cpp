#include "Subtract.h"
#include "Process.h"
#include <string>

Subtract::Subtract(const std::string& dest, int16_t op1, int16_t op2)
    : destVar(dest), operand1(op1), operand2(op2) {
}

Subtract::Subtract(const std::string& dest, int16_t op1)
    : destVar(dest), operand1(op1), operand2(0) {
}

Subtract::Subtract(const std::string& dest, std::nullopt_t, int16_t op2)
    : destVar(dest), operand1(0), operand2(op2) {
}

Subtract::Subtract(const std::string& dest)
    : destVar(dest), operand1(0), operand2(0) {
}

void Subtract::execute(Process& process) {
    int16_t result = operand1 - operand2;

    process.log("Subtracted " + std::to_string(operand1) + " - " +
        std::to_string(operand2) + " = " +
        std::to_string(result));
}

InstructionType Subtract::getType() const {
    return InstructionType::SUBTRACT;
}