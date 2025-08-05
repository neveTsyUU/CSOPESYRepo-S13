#include "Add.h"
#include "Process.h"
#include <string>

// ADD varA 1 1
Add::Add(const std::string& dest, int16_t op1, int16_t op2)
    : destVar(dest), operand1(op1), operand2(op2) {
    mode = "value+value";
}

Add::Add(bool isCosmeticFlag)
{
    isCosmetic = isCosmeticFlag;
}

// ADD varA 1 varB
Add::Add(const std::string& dest, int16_t op1, const std::string& op2)
    : destVar(dest), operand1(op1), operand2(0), value1FromProcess(op2) {
    mode = "value+var";
}

// ADD varA varA 1
Add::Add(const std::string& dest, const std::string& op1, int16_t op2)
    : destVar(dest), operand1(0), operand2(op2), value0FromProcess(op1) {
    mode = "var+value";
}

// ADD varA varA varB
Add::Add(const std::string& dest, const std::string& op1, const std::string& op2)
    : destVar(dest), operand1(0), operand2(0), value0FromProcess(op1), value1FromProcess(op2) {
    mode = "var+var";
}

void Add::execute(Process& process) {
    if (isCosmetic) {
        return;
    }
    if (mode == "value+value") {
        process.add(destVar, mode, "", "", operand1, operand2);
    }
    else if (mode == "value+var") {
        process.add(destVar, mode, "", value1FromProcess, operand1, 0);
    }
    else if (mode == "var+value") {
        process.add(destVar, mode, value0FromProcess, "", 0, operand2);
    }
    else if (mode == "var+var") {
        process.add(destVar, mode, value0FromProcess, value1FromProcess, 0, 0);
    }
    else {
        return;
    }
}


InstructionType Add::getType() const {
    return InstructionType::ADD;
}
