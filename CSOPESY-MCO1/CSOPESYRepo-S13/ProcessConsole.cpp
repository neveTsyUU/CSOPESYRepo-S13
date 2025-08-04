#include "ProcessConsole.h"
#include "ConsoleManager.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>

ProcessConsole::ProcessConsole(std::shared_ptr<ConsoleManager> mgr, const std::string& name, std::shared_ptr<Process> proc)
    : ConsoleTemplate(), manager(mgr), assignedProcess(std::move(proc)) {
}

void ProcessConsole::onEnabled() {
    std::cout << "*=========================================================================*\n";
    std::cout << "Process: " << assignedProcess->getName() << "\n";
    std::cout << "ID: " << assignedProcess->getId() << "\n";
    std::cout << "Created on: " << assignedProcess->getFormattedCreationTime() << "\n";
    std::cout << "+=========================================================================*\n";
    //std::cout << "Instruction  : " << assignedProcess->getCurrentInstructionName() << "\n";
    std::cout << "Current instruction line: " << assignedProcess->getCurrentLine() << "\n";
    std::cout << "Lines of code: " << assignedProcess->getTotalLines() << "\n";
    std::cout << "+=========================================================================*\n";
    /*display();*/
    std::cout << "Type 'exit' to return to MainConsole, 'run' to execute a line, or 'status' to view details.\n";
}

void ProcessConsole::display() {
    //std::cout << "Process: " << assignedProcess->getName() << "\n";
    //std::cout << "Created on: " << assignedProcess->getFormattedCreationTime() << "\n\n";

    //std::cout << "Current instruction line: " << assignedProcess->getCurrentLine() << "\n";
    //std::cout << "Lines of code: " << assignedProcess->getTotalLines() << "\n";
}

void ProcessConsole::process() {
    std::string input;
    std::cout << assignedProcess->getName() << "> ";
    std::getline(std::cin, input);

    if (input == "exit") {
        system("cls");
        manager->setActiveConsole("Main");
    }
    else if (input == "status") {
        showStatus();
    }
    else if (input == "process-smi") {
        processSMI();
    }
    else {
        std::cout << "Unknown command.\n\n";
    }
}

bool ProcessConsole::isProcessFinished()
{
    return assignedProcess->isFinished();
}

void ProcessConsole::processSMI() const
{
    if (!assignedProcess->isFinished()) {
        std::cout << "\n";
        std::cout << "=========================" << "\n";
        std::cout << "Process: " << assignedProcess->getName() << "\n";
        std::cout << "ID: " << assignedProcess->getId() << "\n";
        std::cout << "Created on: " << assignedProcess->getFormattedCreationTime() << "\n";
        std::cout << "=========================" << "\n";
        std::cout << "Print Log:" << "\n";
        assignedProcess->printLogs();
        std::cout << "=========================" << "\n";
        std::cout << "Current instruction line : " << assignedProcess->getCurrentLine() << "\n";
        std::cout << "Lines of code: " << assignedProcess->getTotalLines() << "\n";
        std::cout << "Assigned CPU: " << assignedProcess->getAssignedCPUId() << "\n";
        std::cout << "=========================" << "\n\n";
    }
    else {
        std::cout << "\n";
        std::cout << "=========================" << "\n";
        std::cout << "Process: " << assignedProcess->getName() << "\n";
        std::cout << "ID: " << assignedProcess->getId() << "\n";
        std::cout << "Finished on: " << assignedProcess->getFormattedFinishTime() << "\n";
        assignedProcess->printLogs();
        std::cout << "\n";
        //std::cout << "Current instruction line : " << assignedProcess->getCurrentLine() << "\n";
        //std::cout << "Lines of code: " << assignedProcess->getTotalLines() << "\n";
        //std::cout << "Assigned CPU: " << assignedProcess->getAssignedCPUId() << "\n";
        std::cout << "Finished!" << std::endl;
        std::cout << "=========================" << "\n\n";
    }

}

void ProcessConsole::showStatus() const {
    std::cout << "\n--- Process Status ---\n";
    std::cout << "Name         : " << assignedProcess->getName() << "\n";
    std::cout << "ID           : " << assignedProcess->getId() << "\n";
    std::cout << "CPU ID       : " << (assignedProcess->getState() == Process::State::Finished ? "N/A" : std::to_string(assignedProcess->getId())) << "\n";
    std::cout << "Current Line : " << assignedProcess->getCurrentLine() << "\n";
    std::cout << "Total Lines  : " << assignedProcess->getTotalLines() << "\n";
    //std::cout << "Instruction  : " << assignedProcess->getCurrentInstructionName() << "\n";
    std::cout << "Created      : " << assignedProcess->getFormattedCreationTime() << "\n";
    std::cout << "Finished     : " << assignedProcess->getFormattedFinishTime() << "\n";
    std::cout << "State        : ";

    switch (assignedProcess->getState()) {
    case Process::State::Waiting: std::cout << "Waiting"; break;
    case Process::State::Ready:   std::cout << "Ready"; break;
    case Process::State::Running: std::cout << "Running"; break;
    case Process::State::Finished:std::cout << "Finished"; break;
    }

    std::cout << "\n------------------------\n\n";
}

void ProcessConsole::runSingleStep() {
    if (assignedProcess->isFinished()) {
        std::cout << "Process already finished.\n";
        return;
    }

    std::cout << "Executing line " << assignedProcess->getCurrentLine() + 1 << "...\n";
    assignedProcess->advanceLine();

    if (assignedProcess->isFinished()) {
        assignedProcess->markFinished();
        std::cout << "Process completed.\n";
    }
    else {
        std::cout << "Line executed. Now at line " << assignedProcess->getCurrentLine() << ".\n";
    }
}