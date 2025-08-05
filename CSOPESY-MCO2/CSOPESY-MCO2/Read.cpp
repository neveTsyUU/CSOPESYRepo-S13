#include "Read.h"
#include "Process.h"
#include <sstream>

// e.g. READ varC 0x500
Read::Read(const std::string& dest, const std::string& addressStr)
    : destVar(dest), addressStr(addressStr) {
}

void Read::execute(Process& process)
{
    // [READ] [destVar] [addressStr]
    process.read(addressStr, destVar);
}


InstructionType Read::getType() const {
    return InstructionType::READ;
}
