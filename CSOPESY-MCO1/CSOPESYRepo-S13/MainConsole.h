#pragma once
#include "ConsoleTemplate.h"
#include "ConsoleManager.h"
#include "Scheduler.h"
#include "Process.h"
#include "ProcessConsole.h"
#include "Config.h"
#include <string>

class MainConsole : public ConsoleTemplate {
public:
    MainConsole(std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched);

    void onEnabled() override;
    void display() override;
    void process() override;

private:
    std::shared_ptr<ConsoleManager> manager;
    std::shared_ptr<Config> config;
    std::shared_ptr<Scheduler> scheduler;
    //Scheduler& scheduler;  
    uint32_t currentProcessId = 0;

    void handleCommand(const std::string& input);
    void printHeader() const;
    void printCommandList() const;
};