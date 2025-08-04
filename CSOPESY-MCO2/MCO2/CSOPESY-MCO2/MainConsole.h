#pragma once

#include "ConsoleTemplate.h"
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "Process.h"
#include "ProcessConsole.h"
#include "PagingMemoryManager.h"
#include "Config.h"
#include <string>

class MainConsole : public ConsoleTemplate {
public:
    MainConsole(std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched, std::shared_ptr<PagingMemoryManager> pmngr);

    void onEnabled() override;
    void display() override;
    void process() override;

private:
    std::shared_ptr<ConsoleManager> manager;
    std::shared_ptr<Config> config;
    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<PagingMemoryManager> pagingManager;
    //Scheduler& scheduler;  
    uint32_t currentProcessId = 0;

    void handleCommand(const std::string& input);
    std::vector<std::string> tokenizeCommand(const std::string& command);

    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    std::vector<std::string> processCommand(const std::string& cmd);
    void cleanTokens(std::vector<std::string>& tokens);
    void printHeader() const;
    void printCommandList() const;
};