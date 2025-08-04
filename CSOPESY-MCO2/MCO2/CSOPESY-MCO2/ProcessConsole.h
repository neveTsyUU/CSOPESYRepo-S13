#pragma once

#include "ConsoleTemplate.h"
#include "Process.h"
#include <memory>
#include <string>

class ConsoleManager;

class ProcessConsole : public ConsoleTemplate {
public:
    ProcessConsole(std::shared_ptr<ConsoleManager> mgr, const std::string& name, std::shared_ptr<Process> proc);

    void onEnabled() override;
    void display() override;
    void process() override;
    bool isProcessFinished();
    bool isProcessStopped();

    std::string getStopTime();
    std::string getInvalidMemoryAddressAccess();

private:
    std::shared_ptr<ConsoleManager> manager;
    std::shared_ptr<Process> assignedProcess;
    
    void processSMI() const;
    void showStatus() const;
    void runSingleStep();
};
