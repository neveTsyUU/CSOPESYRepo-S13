#include "Add.h"
#include "Process.h"
#include <string>

Add::Add(const std::string& dest, int16_t op1, int16_t op2)
    : destVar(dest), operand1(op1), operand2(op2) {
}

Add::Add(const std::string& dest, int16_t op1)
    : destVar(dest), operand1(op1), operand2(0) {
}

Add::Add(const std::string& dest, std::nullopt_t, int16_t op2)
    : destVar(dest), operand1(0), operand2(op2) {
}

Add::Add(const std::string& dest)
    : destVar(dest), operand1(0), operand2(0) {
}

void Add::execute(Process& process) {
    int16_t result = operand1 + operand2;
    //process.log("Added " + std::to_string(operand1) + " + " + std::to_string(operand2) + " = " + std::to_string(result));
}

InstructionType Add::getType() const {
    return InstructionType::ADD;
}