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
    oss << std::put_time(&tm, "%m/%d/%Y, %I:%M:%S %p");
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

/*

std::string Process::getInstructionNameAt(std::uint32_t index) const {
    if (index < instructionName.size()) {
        return instructionName[index];
    }
    return "Invalid Index";
}

std::string Process::getCurrentInstructionName() const {
    return getInstructionNameAt(currentLineOfCode);
}
*/

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

        int instructionType = std::rand() % 15; // 0 to 14 only

        if (instructionType == 0) {
            instructionList.push_back(std::make_shared<Print>());
            //instructionName.push_back("PRINT");
        }
        else if (instructionType == 1) {
            instructionList.push_back(std::make_shared<Print>("This is a message"));
            //instructionName.push_back("PRINT");
        }
        else if (instructionType == 2) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            std::string msg = "Displaying value of variable " + std::string(1, varName) + ": ";
            instructionList.push_back(std::make_shared<Print>(msg, value));
            //instructionName.push_back("PRINT");
        }
        else if (instructionType == 3) {
            char varName = getRandomVariableName();
            instructionList.push_back(std::make_shared<Declare>(std::string(1, varName)));
            //instructionName.push_back("DECLARE");
        }
        else if (instructionType == 4) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            instructionList.push_back(std::make_shared<Declare>(std::string(1, varName), value));
            //instructionName.push_back("DECLARE");
        }
        else if (instructionType == 5) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Add>(std::string(1, dest), op1, op2));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 6) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Add>(std::string(1, dest), op1));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 7) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            instructionList.push_back(std::make_shared<Add>(std::string(1, dest), n, op2));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 8) {
            // ADD(dest)
            char dest = getRandomVariableName();
            instructionList.push_back(std::make_shared<Add>(std::string(1, dest)));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 9) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), op1, op2));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 10) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            instructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), op1));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 11) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            instructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), n, op2));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 12) {
            // ADD(dest)
            char dest = getRandomVariableName();
            instructionList.push_back(std::make_shared<Subtract>(std::string(1, dest)));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 13) {
            // ADD(dest)
            uint8_t sleepTime = getRandomUint8();
            instructionList.push_back(std::make_shared<Sleep>(sleepTime));
            //instructionName.push_back("SLEEP");
        }
        else if (instructionType == 14) {
            uint32_t repeatCount = (std::rand() % 4) + 1;
            instructionList.push_back(std::make_shared<For>(repeatCount));
        }
    }
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
    //static std::mt19937 rng(std::random_device{}());
    //static std::uniform_int_distribution<uint16_t> dist(0, UINT8_MAX);
    //return dist(rng);
    return 8;
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