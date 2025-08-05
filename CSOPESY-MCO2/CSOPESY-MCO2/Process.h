#pragma once

#include "CommandTemplate.h"
#include "Config.h"
#include "Print.h"
#include "Declare.h"
#include "Add.h"
#include "Subtract.h"
#include "Sleep.h"
#include "Write.h"
#include "Read.h"
#include <string>
#include <ctime>
#include <cstdint>
#include <vector>
#include <random>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <unordered_map>

class Process {
public:
    typedef std::string String;

    enum class State {
        Waiting,
        Ready,
        Running,
        Finished,
        Stopped
    };

    Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg);
    Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::uint32_t memorySize);
    Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::vector<String> commands);
    Process(std::uint32_t id, const String& name, std::shared_ptr<Config> cfg, std::uint32_t memorySize, std::vector<String> commands);
    Process(std::uint32_t id, const String& name, std::uint32_t currentLine, std::uint32_t totalLines, std::time_t creationTime, std::uint32_t memorySize, std::uint32_t frameSize, std::uint32_t numPages);

    std::uint32_t getId() const;
    const String& getName() const;
    std::uint32_t getCurrentLine() const;
    std::uint32_t getTotalLines() const;
    String getFormattedCreationTime() const;
    String getFormattedFinishTime() const;
    String getFormattedShutdownTime() const;
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
    std::uint32_t getRandomProcessSize();

    std::uint32_t getProcessMemorySize();
    std::uint32_t getProcessFrameSize();
    std::uint32_t getNumberOfPages();
    std::time_t getCreationTime();

    void createCommandsFromString();

    std::string trim(const std::string& str);
    /*   std::vector<std::string> split(const std::string& str, char delimiter);*/
    std::vector<std::string> processCommand(const std::string& cmd);
    void cleanTokens(std::vector<std::string>& tokens);

    bool isMemoryAddress(const std::string& token);
    bool isDecimalLiteral(const std::string& token);
    bool isVariable(const std::string& token);

    void visualizeMemoryMap();
    void visualizeSymbolTable();

    void declareVariable(std::string var, int16_t value);
    void add(std::string var, std::string mode, std::string var1, std::string var2, int16_t val1, int16_t val2);
    void substract(std::string var, std::string mode, std::string var1, std::string var2, int16_t val1, int16_t val2);
    void write(std::string mode, std::string address, int16_t val, std::string var);
    void read(std::string address, std::string var);
    // After DECLARE varA 10
    std::unordered_map<std::string, int16_t> symbolTable;
    // After WRITE 0x500 varA
    std::unordered_map<std::string, int16_t> memory;
    bool shutdownFlag = false;

    bool isShutdown();

    void setShutdownTime();
    void setInvalidMemoryAddress(std::string address);
    std::string getInvalidMemoryAddress();
private:
    std::uint32_t processMemorySize;
    std::uint32_t processFrameSize;
    std::uint32_t numberOfPages;
    std::vector<String> processCommands;
    std::vector<std::string> logs;
    std::time_t shutdownTime;
    std::string invalidMemoryAddress;


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
    uint8_t sleepTicksRemaining = 0;
};
