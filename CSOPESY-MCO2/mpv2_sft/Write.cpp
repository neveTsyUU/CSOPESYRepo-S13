#include "Write.h"
#include "Process.h"
#include <sstream>

// Constant value
Write::Write(const std::string& addressStr, int16_t value)
    : addressStr(addressStr), constantValue(value) {
    mode = "address+value";
}

// Value from process variable
Write::Write(const std::string& addressStr, const std::string& varName)
    : addressStr(addressStr), valueFromProcess(varName) {
    mode = "address+var";
}

void Write::execute(Process& process) {
    /*std::cout << "Exec: " << addressStr << std::endl;*/

    if (mode == "address+value") {
        // Write a constant value directly to memory
        process.write("MEMORY", addressStr, constantValue, "");
    }
    else if (mode == "address+var") {
        // Fetch the value from the symbol table using variable name
        if (process.symbolTable.find(valueFromProcess) != process.symbolTable.end()) {
            int16_t val = process.symbolTable[valueFromProcess];
            process.write("MEMORY", addressStr, val, "");
        }
        else {
            return;
            /*std::cerr << "Variable " << valueFromProcess << " not found in symbol table.\n";*/
        }
    }
    else {
        return;
        /*std::cerr << "Invalid write mode in Write::execute: " << mode << std::endl;*/
    }
}


InstructionType Write::getType() const {
    return InstructionType::WRITE;
}
