#pragma once
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ConsoleTemplate.h"
#include "ProcessConsole.h"

class ConsoleManager {
public:
    ConsoleManager() = default;

    void addConsole(const std::string& name, std::unique_ptr<ConsoleTemplate> console);
    void setActiveConsole(const std::string& name);
    void run();
    void mainPreInitializationHeader();
    void mainPreInitializationDisplayInput();
    void initializationProcess();

    std::vector<std::string> listConsoleNames() const;
    bool consoleExists(const std::string& name) const;
    size_t getConsoleCount() const;
    void initialize();
    bool initialized();
    ProcessConsole* getProcessConsole(const std::string& name);

private:
    bool isInitialized = false;
    std::unordered_map<std::string, std::unique_ptr<ConsoleTemplate>> consoles;
    ConsoleTemplate* current = nullptr;
};