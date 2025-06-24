#pragma once

#include "CommandTemplate.h"
#include "Config.h"
#include "Print.h"
#include "Declare.h"
#include "Add.h"
#include "Subtract.h"
#include "Sleep.h"
#include <string>
#include <ctime>
#include <cstdint>
#include <vector>
#include <random>

class Process {
public:
    typedef std::string String;

    enum class State {
        Waiting,
        Ready,
        Running,
        Finished
    };

    Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg);

    std::uint32_t getId() const;
    const String& getName() const;
    std::uint32_t getCurrentLine() const;
    std::uint32_t getTotalLines() const;
    String getFormattedCreationTime() const;
    String getFormattedFinishTime() const;
    //std::string getInstructionNameAt(std::uint32_t index) const;
    //std::string getCurrentInstructionName() const;
    std::string getCurrentFormattedTime();
    bool isFinished() const;
    void assignCpuId(int32_t cpuID);
    void markFinished();
    bool executeCurrentLine();
    void createRandomSetOfCommands();
    std::int32_t getAssignedCPUId();
    std::uint16_t getRandomUint16();
    std::uint8_t getRandomUint8();
    char getRandomVariableName();
    void setSleepTicksRemaining(std::uint8_t sleepTicks);

    void advanceLine();
    void setState(State s);
    State getState() const;
    void log(const std::string& message);
    const std::vector<std::string>& getLogs() const;
    void printLogs();


private:
    std::vector<std::string> logs;
    std::shared_ptr<Config> config;
    std::uint32_t id;
    std::int32_t cpuId = -1;
    String name;
    std::uint32_t currentLineOfCode = 0;
    std::uint32_t totalLinesOfCode;
    std::time_t creationTime;
    std::time_t finishTime = 0;
    State state = State::Ready;
    std::vector<std::shared_ptr<CommandTemplate>> instructionList;
    std::vector<std::string> instructionName;
    uint8_t sleepTicksRemaining = 0;
};
