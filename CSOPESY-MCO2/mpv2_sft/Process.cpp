#include "Process.h"
#include "ConsoleManager.h"
#include <iomanip>
#include <sstream>
#include <ctime>
#include <iostream>

Process::Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg)
    : id(id),
    name(name),
    creationTime(std::time(nullptr)),
    finishTime(0),
    state(State::Ready),
    cpuId(-1),
    currentLineOfCode(0),
    config(cfg) {
    createRandomSetOfCommands();
    totalLinesOfCode = static_cast<std::uint32_t>(instructionList.size());
    processFrameSize = config->getMemPerFrame();
    processMemorySize = getRandomProcessSize();
    numberOfPages = (processMemorySize + processFrameSize - 1) / processFrameSize;

    std::vector<std::string> allowedAddresses = {
    "0x100", "0x200", "0x300", "0x400", "0x500",
    "0x600", "0x700", "0x800", "0x900", "0x1000",
    "0x1100", "0x1200", "0x1300", "0x1400", "0x1500",
    "0x1600", "0x1700", "0x1800", "0x1900", "0x2000",
    "0x2100", "0x2200", "0x2300", "0x2400", "0x2500",
    "0x2600", "0x2700", "0x2800", "0x2900", "0x3000",
    "0x3100", "0x3200"
    };

    for (const auto& addr : allowedAddresses) {
        memory[addr] = 0;
    }
}

Process::Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::uint32_t memorySize)
    : id(id),
    name(name),
    creationTime(std::time(nullptr)),
    finishTime(0),
    state(State::Ready),
    cpuId(-1),
    currentLineOfCode(0),
    config(cfg),
    processMemorySize(memorySize) {
    createRandomSetOfCommands();
    totalLinesOfCode = static_cast<std::uint32_t>(processCommands.size());
    processFrameSize = config->getMemPerFrame();
    numberOfPages = (processMemorySize + processFrameSize - 1) / processFrameSize;

    std::vector<std::string> allowedAddresses = {
  "0x100", "0x200", "0x300", "0x400", "0x500",
  "0x600", "0x700", "0x800", "0x900", "0x1000",
  "0x1100", "0x1200", "0x1300", "0x1400", "0x1500",
  "0x1600", "0x1700", "0x1800", "0x1900", "0x2000",
  "0x2100", "0x2200", "0x2300", "0x2400", "0x2500",
  "0x2600", "0x2700", "0x2800", "0x2900", "0x3000",
  "0x3100", "0x3200"
    };

    for (const auto& addr : allowedAddresses) {
        memory[addr] = 0;
    }
}

Process::Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::vector<String> commands)
    : id(id),
    name(name),
    creationTime(std::time(nullptr)),
    finishTime(0),
    state(State::Ready),
    cpuId(-1),
    currentLineOfCode(0),
    config(cfg),
    processCommands(commands) {
    createCommandsFromString();
    totalLinesOfCode = static_cast<std::uint32_t>(processCommands.size());
    processFrameSize = config->getMemPerFrame();
    processMemorySize = getRandomProcessSize();
    numberOfPages = (processMemorySize + processFrameSize - 1) / processFrameSize;

    std::vector<std::string> allowedAddresses = {
    "0x100", "0x200", "0x300", "0x400", "0x500",
    "0x600", "0x700", "0x800", "0x900", "0x1000",
    "0x1100", "0x1200", "0x1300", "0x1400", "0x1500",
    "0x1600", "0x1700", "0x1800", "0x1900", "0x2000",
    "0x2100", "0x2200", "0x2300", "0x2400", "0x2500",
    "0x2600", "0x2700", "0x2800", "0x2900", "0x3000",
    "0x3100", "0x3200"
    };

    for (const auto& addr : allowedAddresses) {
        memory[addr] = 0;
    }
}

Process::Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::uint32_t memorySize, std::vector<String> commands)
    : id(id),
    name(name),
    creationTime(std::time(nullptr)),
    finishTime(0),
    state(State::Ready),
    cpuId(-1),
    currentLineOfCode(0),
    config(cfg),
    processMemorySize(memorySize),
    processCommands(commands) {
    createCommandsFromString();
    totalLinesOfCode = static_cast<std::uint32_t>(instructionList.size());
    processFrameSize = config->getMemPerFrame();
    numberOfPages = (processMemorySize + processFrameSize - 1) / processFrameSize;

    std::vector<std::string> allowedAddresses = {
    "0x100", "0x200", "0x300", "0x400", "0x500",
    "0x600", "0x700", "0x800", "0x900", "0x1000",
    "0x1100", "0x1200", "0x1300", "0x1400", "0x1500",
    "0x1600", "0x1700", "0x1800", "0x1900", "0x2000",
    "0x2100", "0x2200", "0x2300", "0x2400", "0x2500",
    "0x2600", "0x2700", "0x2800", "0x2900", "0x3000",
    "0x3100", "0x3200"
    };

    for (const auto& addr : allowedAddresses) {
        memory[addr] = 0;
    }
}

Process::Process(std::uint32_t id, const String& name, std::uint32_t currentLine, std::uint32_t totalLines, std::time_t creationTime, std::uint32_t memorySize, std::uint32_t frameSize, std::uint32_t numPages)
    : id(id),
    name(name),
    currentLineOfCode(currentLine),
    totalLinesOfCode(totalLines),
    creationTime(creationTime),
    processMemorySize(memorySize),
    processFrameSize(frameSize),
    numberOfPages(numPages) {
}


std::uint32_t Process::getId() const {
    return id;
}

const Process::String& Process::getName() const {
    return name;
}

std::uint32_t Process::getCurrentLine() const {
    return currentLineOfCode;
}

std::uint32_t Process::getTotalLines() const {
    return totalLinesOfCode;
}

Process::String Process::getFormattedCreationTime() const {
    std::tm tm{};
    localtime_s(&tm, &creationTime);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");  // 24-hour format
    return oss.str();
}

Process::String Process::getFormattedFinishTime() const {
    if (finishTime == 0) return "N/A";

    std::tm tm{};
    localtime_s(&tm, &finishTime);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}

Process::String Process::getFormattedShutdownTime() const
{
    std::tm tm{};
    localtime_s(&tm, &shutdownTime);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}

std::string Process::getCurrentFormattedTime()
{
    std::time_t now = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &now);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%m/%d/%Y, %I:%M:%S %p");
    return oss.str();
}

bool Process::isFinished() const {
    return currentLineOfCode >= totalLinesOfCode;
}

void Process::assignCpuId(int32_t cpuID)
{
    cpuId = cpuID;
}

void Process::markFinished()
{
    cpuId = -1;
    finishTime = std::time(nullptr);
    state = State::Finished;
}

void Process::createRandomSetOfCommands()
{
    uint32_t low = config->getMinIns();
    uint32_t high = config->getMaxIns();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(low, high);

    uint32_t numInstructions = dist(gen);

    instructionList.clear();


    for (uint32_t i = 0; i < numInstructions; ++i)
    {
        //static std::mt19937 rng(std::random_device{}());
        //static std::uniform_int_distribution<int> dist(0, 13);
        //int instructionType = dist(rng);

        int instructionType = std::rand() % 14; // 0 to 13 inclusiv

        if (instructionType == 0) {
            instructionList.push_back(std::make_shared<Print>(true));
        }
        else if (instructionType == 1) {
            instructionList.push_back(std::make_shared<Print>(true));
        }
        else if (instructionType == 2) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            std::string msg = "Value from: " + std::string(1, varName) + " = ";
            instructionList.push_back(std::make_shared<Print>(true));
        }
        else if (instructionType == 3) {
            char varName = getRandomVariableName();
            instructionList.push_back(std::make_shared<Declare>(true));
        }
        else if (instructionType == 4) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            instructionList.push_back(std::make_shared<Declare>(true));
        }
        else if (instructionType == 5) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Add>(true));
        }
        else if (instructionType == 6) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Add>(true));
        }
        else if (instructionType == 7) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            instructionList.push_back(std::make_shared<Add>(true));
        }
        else if (instructionType == 8) {
            // ADD(dest)
            char dest = getRandomVariableName();
            instructionList.push_back(std::make_shared<Add>(true));
        }
        else if (instructionType == 9) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Subtract>(true));
        }
        else if (instructionType == 10) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Subtract>(true));
        }
        else if (instructionType == 11) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            instructionList.push_back(std::make_shared<Subtract>(true));
        }
        else if (instructionType == 12) {
            // ADD(dest)
            char dest = getRandomVariableName();
            instructionList.push_back(std::make_shared<Subtract>(true));
        }
        else if (instructionType == 13) {
            // ADD(dest)
            uint8_t sleepTime = getRandomUint8();
            instructionList.push_back(std::make_shared<Sleep>(sleepTime));
        }
    }
}

void Process::createCommandsFromString()
{
    //    SLEEP 8
    //    SUB varA varA varB
    //    SUB varA  0   varB
    //    SUB varA varA  1
    //    SUB varA  1    1
    //    DECLARE varA 10
    //    DECLARE varB 5
    //    ADD varA varA varB
    //    ADD varA  0   varB
    //    ADD varA varA  1
    //    ADD varA  1    1
    //    WRITE 0x500 varA
    //    READ varC 0x500
    //    PRINT("Result: " + varC)
    //    DECLARE varA 10


        // --- Declarations ---
    // [DECLARE] [varA] [10]
    // [DECLARE] [varB] [5]
    // [DECLARE] [varA] [10]
    // [DECLARE] [varB] [5]
    // [DECLARE] [varA] [10]
    // --- Arithmetic: ADD ---
    // [ADD]     [varA] [varA] [varB]
    // [ADD]     [varA] [0]    [varB]
    // [ADD]     [varA] [varA] [1]
    // [ADD]     [varA] [1]    [1]
    // --- Arithmetic: SUB ---
    // [SUB]     [varA] [varA] [varB]
    // [SUB]     [varA] [0]    [varB]
    // [SUB]     [varA] [varA] [1]
    // [SUB]     [varA] [1]    [1]
    // --- Memory I/O ---
    // [WRITE]   [0x500] [varA]
    // [WRITE]   [0x500] [varA]
    // [WRITE]   [0x500] [12]
    // [READ]    [varC] [0x500]
    // [READ]    [varC] [0x500]
    // --- Output ---
    // [PRINT]   [Result:] [+] [varC]
    // [PRINT]   [Result:] [+] [varC]
    // --- Sleep ---
    // [SLEEP]   [8]

  /*  std::cout << "Commands: " << processCommands.size() << std::endl;*/


    for (const auto& cmd : processCommands) {
        std::vector<std::string> tokens = processCommand(cmd);
        cleanTokens(tokens);

        if (tokens.size() == 2) {
            if (tokens[0] == "SLEEP" && isDecimalLiteral(tokens[1])) {
                uint8_t sleepTime = static_cast<uint8_t>(std::stoi(tokens[1]));
                instructionList.push_back(std::make_shared<Sleep>(sleepTime));
            }
        }

        else if (tokens.size() == 3) {
            if (tokens[0] == "DECLARE" && isVariable(tokens[1]) && isDecimalLiteral(tokens[2])) {
                uint16_t val = static_cast<uint16_t>(std::stoi(tokens[2]));
                instructionList.push_back(std::make_shared<Declare>(tokens[1], val));
            }
            else if (tokens[0] == "WRITE" && isMemoryAddress(tokens[1]) && isVariable(tokens[2])) {
                instructionList.push_back(std::make_shared<Write>(tokens[1], tokens[2]));
                /*std::cout << "THIS1 " << tokens[1] << std::endl;*/
            }
            else if (tokens[0] == "WRITE" && isMemoryAddress(tokens[1]) && isDecimalLiteral(tokens[2])) {
                int16_t val = static_cast<int16_t>(std::stoi(tokens[2]));
                instructionList.push_back(std::make_shared<Write>(tokens[1], val));
                /*std::cout << "THIS2 " << tokens[1] << std::endl;*/
            }
            else if (tokens[0] == "READ" && isVariable(tokens[1]) && isMemoryAddress(tokens[2])) {
                instructionList.push_back(std::make_shared<Read>(tokens[1], tokens[2]));
                /*std::cout << "THIS3 " << tokens[2] << std::endl;*/
            }
        }

        else if (tokens.size() == 4) {
            if (tokens[0] == "ADD" &&
                isVariable(tokens[1]) &&
                isVariable(tokens[2]) &&
                isVariable(tokens[3])) {
                instructionList.push_back(std::make_shared<Add>(tokens[1], tokens[2], tokens[3]));
            }

            else if (tokens[0] == "ADD" &&
                isVariable(tokens[1]) &&
                isDecimalLiteral(tokens[2]) &&
                isVariable(tokens[3])) {
                int16_t op1 = static_cast<int16_t>(std::stoi(tokens[2]));
                instructionList.push_back(std::make_shared<Add>(tokens[1], op1, tokens[3]));
            }

            else if (tokens[0] == "ADD" &&
                isVariable(tokens[1]) &&
                isVariable(tokens[2]) &&
                isDecimalLiteral(tokens[3])) {
                int16_t op2 = static_cast<int16_t>(std::stoi(tokens[3]));
                instructionList.push_back(std::make_shared<Add>(tokens[1], tokens[2], op2));
            }

            else if (tokens[0] == "ADD" &&
                isVariable(tokens[1]) &&
                isDecimalLiteral(tokens[2]) &&
                isDecimalLiteral(tokens[3])) {
                int16_t op1 = static_cast<int16_t>(std::stoi(tokens[2]));
                int16_t op2 = static_cast<int16_t>(std::stoi(tokens[3]));
                instructionList.push_back(std::make_shared<Add>(tokens[1], op1, op2));
            }

            // --- SUB Variants ---
            else if (tokens[0] == "SUB" &&
                isVariable(tokens[1]) &&
                isVariable(tokens[2]) &&
                isVariable(tokens[3])) {
                instructionList.push_back(std::make_shared<Subtract>(tokens[1], tokens[2], tokens[3]));
            }

            else if (tokens[0] == "SUB" &&
                isVariable(tokens[1]) &&
                isDecimalLiteral(tokens[2]) &&
                isVariable(tokens[3])) {
                int16_t op1 = static_cast<int16_t>(std::stoi(tokens[2]));
                instructionList.push_back(std::make_shared<Subtract>(tokens[1], op1, tokens[3]));
            }

            else if (tokens[0] == "SUB" &&
                isVariable(tokens[1]) &&
                isVariable(tokens[2]) &&
                isDecimalLiteral(tokens[3])) {
                int16_t op2 = static_cast<int16_t>(std::stoi(tokens[3]));
                instructionList.push_back(std::make_shared<Subtract>(tokens[1], tokens[2], op2));
            }

            else if (tokens[0] == "SUB" &&
                isVariable(tokens[1]) &&
                isDecimalLiteral(tokens[2]) &&
                isDecimalLiteral(tokens[3])) {
                int16_t op1 = static_cast<int16_t>(std::stoi(tokens[2]));
                int16_t op2 = static_cast<int16_t>(std::stoi(tokens[3]));
                instructionList.push_back(std::make_shared<Subtract>(tokens[1], op1, op2));
            }

            else if (tokens[0] == "PRINT" &&
                tokens[2] == "+" &&
                isVariable(tokens[3])) {
                instructionList.push_back(std::make_shared<Print>(tokens[1], tokens[3]));
                // [PRINT] [Result: ] [+] [varC] 
            }
        }
    }
    /*  std::cout << "Instructions size: " << instructionList.size() << std::endl;*/
}


/*  for (const auto& token : tokens) {
      std::cout << "[" << token << "] ";
  }
  std::cout << "\n";*/
std::string Process::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

std::vector<std::string> Process::processCommand(const std::string& cmd)
{
    std::vector<std::string> tokens;

    if (cmd.find("PRINT(") == 0) {
        size_t startQuote = cmd.find('"');
        size_t endQuote = cmd.find('"', startQuote + 1);

        if (startQuote == std::string::npos || endQuote == std::string::npos || endQuote <= startQuote) {
            tokens.push_back("INVALID PRINT");
            return tokens;
        }

        std::string label = cmd.substr(startQuote + 1, endQuote - startQuote - 1);

        // Unescape \" into "
        std::string cleanedLabel;
        for (size_t i = 0; i < label.length(); ++i) {
            if (label[i] == '\\' && i + 1 < label.length() && label[i + 1] == '"') {
                cleanedLabel += '"';
                ++i;
            }
            else {
                cleanedLabel += label[i];
            }
        }

        // Find '+' sign and extract the variable after it
        size_t plusPos = cmd.find('+', endQuote);
        std::string var = trim(cmd.substr(plusPos + 1));

        // Clean up variable (remove potential trailing characters)
        while (!var.empty() && (var.back() == ')' || var.back() == '"' || isspace(var.back()))) {
            var.pop_back();
        }

        tokens.push_back("PRINT");
        tokens.push_back(cleanedLabel);
        tokens.push_back("+");
        tokens.push_back(var);
    }
    else {
        std::stringstream ss(cmd);
        std::string word;
        while (ss >> word) {
            tokens.push_back(word);
        }
    }

    return tokens;
}

void Process::cleanTokens(std::vector<std::string>& tokens)
{
    for (auto& token : tokens) {
        // Remove backslashes and double quotes
        token.erase(std::remove(token.begin(), token.end(), '\\'), token.end());
        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    }
}

bool Process::isMemoryAddress(const std::string& token)
{
    return token.size() > 2 &&
        token[0] == '0' &&
        (token[1] == 'x' || token[1] == 'X') &&
        std::all_of(token.begin() + 2, token.end(), ::isxdigit);
}

bool Process::isDecimalLiteral(const std::string& token)
{
    return std::all_of(token.begin(), token.end(), ::isdigit);
}

bool Process::isVariable(const std::string& token)
{
    return std::all_of(token.begin(), token.end(), ::isalpha);
}

void Process::visualizeMemoryMap()
{
    std::cout << "=== Memory Map for Process " << this->id << " ===\n";

    std::cout << "\n-- Memory Addresses --\n";
    for (const auto& entry : memory) {
        std::cout << "Address: " << entry.first << " -> Value: " << entry.second << "\n";
    }

    std::cout << "===============================\n";
}

void Process::visualizeSymbolTable()
{
    std::cout << "Symbol Table:\n";
    for (const auto& pair : symbolTable) {
        std::cout << "Variable: " << pair.first << " -> Value: " << pair.second << "\n";
    }
}

void Process::declareVariable(std::string var, int16_t value)
{
    if (symbolTable.size() >= 32) {
        //std::cerr << "Error: Maximum number of variables (32) reached. Cannot declare variable: " << var << std::endl;
        return;
    }

    symbolTable[var] = value;
}


void Process::add(std::string var, std::string mode, std::string var1, std::string var2, int16_t val1, int16_t val2)
{
    int16_t result = 0;

    if (mode == "value+value") {
        result = val1 + val2;
    }
    else if (mode == "value+var") {
        if (symbolTable.find(var2) != symbolTable.end()) {
            result = val1 + symbolTable[var2];
        }
        else {
            /*   std::cerr << "Undefined variable: " << var2 << "\n";*/
            return;
        }
    }
    else if (mode == "var+value") {
        if (symbolTable.find(var1) != symbolTable.end()) {
            result = symbolTable[var1] + val2;
        }
        else {
            /*           std::cerr << "Undefined variable: " << var1 << "\n";*/
            return;
        }
    }
    else if (mode == "var+var") {
        if (symbolTable.find(var1) != symbolTable.end() &&
            symbolTable.find(var2) != symbolTable.end()) {
            result = symbolTable[var1] + symbolTable[var2];
        }
        else {
            /*     std::cerr << "Undefined variable(s): " << var1 << " or " << var2 << "\n";*/
            return;
        }
    }
    else {
        /*    std::cerr << "Invalid mode: " << mode << "\n";*/
        return;
    }

    // Store the result
    symbolTable[var] = result;
    //std::cout << "Var A: " << result << std::endl;
}

void Process::substract(std::string var, std::string mode, std::string var1, std::string var2, int16_t val1, int16_t val2)
{
    int16_t result = 0;

    if (mode == "value-value") {
        result = val1 - val2;
    }
    else if (mode == "value-var") {
        if (symbolTable.find(var2) != symbolTable.end()) {
            result = val1 - symbolTable[var2];
        }
        else {
            /*      std::cerr << "Undefined variable: " << var2 << "\n";*/
            return;
        }
    }
    else if (mode == "var-value") {
        if (symbolTable.find(var1) != symbolTable.end()) {
            result = symbolTable[var1] - val2;
        }
        else {
            /*    std::cerr << "Undefined variable: " << var1 << "\n";*/
            return;
        }
    }
    else if (mode == "var-var") {
        if (symbolTable.find(var1) != symbolTable.end() &&
            symbolTable.find(var2) != symbolTable.end()) {
            result = symbolTable[var1] - symbolTable[var2];
        }
        else {
            /*     std::cerr << "Undefined variable(s): " << var1 << " or " << var2 << "\n";*/
            return;
        }
    }
    else {
        /*      std::cerr << "Invalid SUB mode: " << mode << "\n";*/
        return;
    }

    symbolTable[var] = result;
    //std::cout << "Var A: " << result << std::endl;
}

void Process::write(std::string mode, std::string address, int16_t val, std::string var) {
    if (mode == "SYMBOL") {
        symbolTable[var] = val;
    }
    else if (mode == "MEMORY") {
        /*std::cout << "HERE " << address << std::endl;*/
        if (memory.find(address) == memory.end()) {
            /*std::cerr << "Access violation at address: " << address << std::endl;*/
            shutdownFlag = true;
            setShutdownTime();
            setInvalidMemoryAddress(address);
            /*currentLineOfCode = instructionList.size();*/
            return;
        }
        memory[address] = val;
    }
    else {
        /*   std::cerr << "Invalid write mode: " << mode << std::endl;*/
        return;
    }
}

void Process::read(std::string address, std::string var)
{
    // Only proceed if address exists in memory
    if (memory.find(address) == memory.end()) {
        /*std::cerr << "Memory address " << address << " not found. Skipping read.\n";*/
        shutdownFlag = true;
        setShutdownTime();
        setInvalidMemoryAddress(address);
        /*currentLineOfCode = instructionList.size();*/
        return;
    }

    int16_t value = memory[address];

    symbolTable[var] = value;
}

bool Process::isShutdown()
{
    return shutdownFlag;
}

void Process::setShutdownTime()
{
    shutdownTime = std::time(nullptr);
}

void Process::setInvalidMemoryAddress(std::string address)
{
    invalidMemoryAddress = address;
}

std::string Process::getInvalidMemoryAddress()
{
    return invalidMemoryAddress;
}

std::int32_t Process::getAssignedCPUId()
{
    return cpuId;
}

std::uint16_t Process::getRandomUint16()
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<uint16_t> dist(0, UINT16_MAX);
    return dist(rng);
}

std::uint8_t Process::getRandomUint8()
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<uint16_t> dist(0, UINT8_MAX);
    return dist(rng);
    //return 8;
}

char Process::getRandomVariableName()
{
    static const std::string varNames = "abcde";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<std::size_t> dist(0, varNames.size() - 1);
    return varNames[dist(rng)];
}

void Process::setSleepTicksRemaining(std::uint8_t sleepTicks)
{
    sleepTicksRemaining = sleepTicks;
}

bool Process::executeCurrentLine()
{
    if (currentLineOfCode < instructionList.size()) {
        if (sleepTicksRemaining > 0) {
            --sleepTicksRemaining;
            //log("Process sleeping, " + std::to_string(sleepTicksRemaining) + " ticks left");
            return false;
        }
        else {
            instructionList[currentLineOfCode]->execute(*this);
            return true;
        }
    }
    return false;
}



//void Process::executeCurrentLine()
//{
//    if (currentLineOfCode < instructionList.size()) {
//        instructionList[currentLineOfCode]->execute(*this);
//    }
//}

void Process::advanceLine() {
    if (currentLineOfCode < instructionList.size()) {
        currentLineOfCode++;
    }
    else {
    }
}

void Process::setState(State s) {
    state = s;
}

Process::State Process::getState() const {
    return state;
}

void Process::log(const std::string& message) {
    logs.push_back("(" + getCurrentFormattedTime() + ") Core:" + std::to_string(cpuId) + " " + message);
}

const std::vector<std::string>& Process::getLogs() const {
    return logs;
}

void Process::printLogs()
{
    std::stringstream ss;
    for (const auto& logEntry : logs) {
        ss << logEntry << '\n';
    }
    std::cout << ss.str();
}

std::uint32_t Process::getRandomProcessSize() {
    std::uint32_t min = std::max(config->getMinMemPerProc(), static_cast<std::uint32_t>(64));
    std::uint32_t max = std::min(config->getMaxMemPerProc(), static_cast<std::uint32_t>(65536));

    std::vector<std::uint32_t> validSizes;
    for (std::uint32_t i = 6; i <= 16; ++i) {
        std::uint32_t val = 1u << i;
        if (val >= min && val <= max) {
            validSizes.push_back(val);
        }
    }

    if (validSizes.empty()) {
        throw std::runtime_error("No valid power-of-2 process sizes in the given range.");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, validSizes.size() - 1);

    return validSizes[dist(gen)];
}

std::uint32_t Process::getProcessMemorySize()
{
    return processMemorySize;
}

std::uint32_t Process::getProcessFrameSize()
{
    return processFrameSize;
}

std::uint32_t Process::getNumberOfPages()
{
    return numberOfPages;
}

std::time_t Process::getCreationTime()
{
    return creationTime;
}