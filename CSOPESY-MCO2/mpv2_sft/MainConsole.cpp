#include "MainConsole.h"
#include "ConsoleManager.h"

#include <iostream>
#include <cstdlib>
#include <string>

MainConsole::MainConsole(std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched, std::shared_ptr<PagingMemoryManager> pmngr)
    : manager(mgr), config(cnfg), scheduler(sched), pagingManager(pmngr) {
}

void MainConsole::onEnabled() {
    std::cout << " ______     ______     ______     ______   ______     ______     __  __    \n";
    std::cout << "/\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   \n";
    std::cout << "\\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  __/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  \n";
    std::cout << " \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ \n";
    std::cout << "  \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ \n\n";
}

void MainConsole::display() {
    std::cout << "[MainConsole]> ";
}

void MainConsole::process() {
    std::string input;
    std::getline(std::cin >> std::ws, input);
    handleCommand(input);
}

void MainConsole::handleCommand(const std::string& input) {
    std::vector<std::string> tokens = tokenizeCommand(input);

    if (input == "exit") {
        std::cout << "Exiting the program...\n";
        exit(0);
    }
    else if (input == "clear") {
        system("cls");
        onEnabled();
    }
    else if (input == "process-smi") {
        pagingManager->processSMI();
    }
    else if (input == "vmstat") {
        pagingManager->vmStat();
    }
    else if (input == "scheduler-start") {
        scheduler->startProcessMaker();
    }
    else if (input == "scheduler-stop") {
        scheduler->stopProcessMaker();
    }
    else if (input == "screen -ls") {
        scheduler->printWorkingAndFinishedProcesses();
    }
    else if (input == "list") {
        std::cout << "*=========================== COMMANDS ===========================*\n";
        std::cout << "| 'exit'         -> Exit the program                            |\n";
        std::cout << "| 'clear'        -> Clear the screen                            |\n";
        std::cout << "| 'list'         -> List available commands                     |\n";
        std::cout << "| 'count'        -> Show total registered consoles              |\n";
        std::cout << "| 'screen -r X'  -> Resume existing screen with name X          |\n";
        std::cout << "*===============================================================*\n";
        std::cout << std::endl;
    }
    else if (tokens.size() == 4) {
        if (tokens[0] == "screen" && tokens[1] == "-s") {
            std::string screenName = tokens[2];
            std::string sizeStr = tokens[3];

            // Convert memory size string to integer
            uint32_t memSize = 0;
            try {
                memSize = std::stoul(sizeStr);
            }
            catch (const std::exception& e) {
                std::cout << "Invalid memory size format.\n";
                return;
            }

            // Validate power of 2 and range
            if (memSize < 64 || memSize > 65536 || (memSize & (memSize - 1)) != 0) {
                std::cout << "Memory size must be a power of 2 between 64 and 65536.\n";
                return;
            }

            if (!manager->consoleExists(screenName)) {
                // Use the specified memory size constructor
                auto newProcess = std::make_shared<Process>(currentProcessId, screenName, config, memSize);
                manager->addConsole(screenName, std::make_unique<ProcessConsole>(manager, screenName, newProcess));
                manager->setActiveConsole(screenName);
                scheduler->enqueueProcess(newProcess);
                currentProcessId += 1;
            }
            else {
                std::cout << "Process with screen name '" << screenName << "' already exists. Use screen -r to resume.\n";
            }
        }
    }
    else if (tokens.size() > 4 && tokens[0] == "screen" && tokens[1] == "-c") {
        std::string screenName = tokens[2];
        uint32_t memorySize = 0;
        size_t commandStartPos;

        bool isMemorySize = std::all_of(tokens[3].begin(), tokens[3].end(), ::isdigit);

        if (isMemorySize) {
            uint32_t parsedSize = static_cast<uint32_t>(std::stoul(tokens[3]));
            if (parsedSize >= 64 && parsedSize <= 65536 && (parsedSize & (parsedSize - 1)) == 0) {
                memorySize = parsedSize;
                commandStartPos = input.find(tokens[3], input.find(screenName)) + tokens[3].length();
            }
            else {
                commandStartPos = input.find(tokens[3], input.find(screenName));
            }
        }
        else {
            commandStartPos = input.find(tokens[3], input.find(screenName));
        }

        while (commandStartPos < input.size() && input[commandStartPos] == ' ') {
            ++commandStartPos;
        }

        std::string inputCommand = input.substr(commandStartPos);
        std::vector<std::string> commands = split(inputCommand, ';');

        std::cout << inputCommand << std::endl;
        std::cout << memorySize << std::endl;

        if (memorySize > 0) {
            if (!manager->consoleExists(screenName)) {
                // Use the specified memory size constructor
                auto newProcess = std::make_shared<Process>(currentProcessId, screenName, config, memorySize, commands);
                manager->addConsole(screenName, std::make_unique<ProcessConsole>(manager, screenName, newProcess));
                manager->setActiveConsole(screenName);
                scheduler->enqueueProcess(newProcess);
                currentProcessId += 1;
            }
            else {
                std::cout << "Process with screen name '" << screenName << "' already exists. Use screen -r to resume.\n";
            }
        }
        else {
            if (!manager->consoleExists(screenName)) {
                // Use the specified memory size constructor
                auto newProcess = std::make_shared<Process>(currentProcessId, screenName, config, commands);
                manager->addConsole(screenName, std::make_unique<ProcessConsole>(manager, screenName, newProcess));
                manager->setActiveConsole(screenName);
                scheduler->enqueueProcess(newProcess);
                currentProcessId += 1;
            }
            else {
                std::cout << "Process with screen name '" << screenName << "' already exists. Use screen -r to resume.\n";
            }
        }
    }
    else if (input.rfind("screen -s ", 0) == 0) {
        std::string screenName = input.substr(10);
        if (!screenName.empty()) {
            if (!manager->consoleExists(screenName)) {
                // Create a new shared process

                auto newProcess = std::make_shared<Process>(currentProcessId, screenName, config);
                manager->addConsole(screenName, std::make_unique<ProcessConsole>(manager, screenName, newProcess));
                manager->setActiveConsole(screenName);
                scheduler->enqueueProcess(newProcess);
                currentProcessId += 1;
            }
            else {
                std::cout << "Process with screen name '" << screenName << "' already exists. Use screen -r to resume.\n";
            }
        }
        else {
            std::cout << "Please specify a valid screen name.\n";
        }
    }
    else if (input.rfind("screen -r ", 0) == 0) {
        std::string screenName = input.substr(10);
        if (!screenName.empty()) {
            if (manager->consoleExists(screenName)) {
                ProcessConsole* pc = manager->getProcessConsole(screenName);
                if (pc->isProcessStopped()) {
                    std::cout << "Process " << screenName
                        << " shut down due to memory access violation error that occurred at "
                        << pc->getStopTime() << ". "
                        << std::showbase << std::hex << std::uppercase
                        << pc->getInvalidMemoryAddressAccess()
                        << " invalid." << std::endl << std::endl;
                }
                else if (pc->isProcessFinished()) {
                    std::cout << "Cannot enter: process finished" << "\n";
                }
                else {
                    system("cls");
                    std::cout << "Resuming '" << screenName << "'...\n";
                    manager->setActiveConsole(screenName);
                }
            }
            else {
                std::cout << "No screen named '" << screenName << "'.\n";
                std::cout << std::endl;
            }
        }
        else {
            std::cout << "Please specify a screen name.\n";
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Unknown command. Type 'list' to see available options.\n";
        std::cout << std::endl;
    }
}

std::vector<std::string> MainConsole::tokenizeCommand(const std::string& command)
{
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> MainConsole::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        token = trim(token);
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

std::string MainConsole::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    if (first == std::string::npos || last == std::string::npos)
        return "";
    return str.substr(first, last - first + 1);
}

std::vector<std::string> MainConsole::processCommand(const std::string& cmd)
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

void MainConsole::cleanTokens(std::vector<std::string>& tokens)
{
    for (auto& token : tokens) {
        // Remove backslashes and double quotes
        token.erase(std::remove(token.begin(), token.end(), '\\'), token.end());
        token.erase(std::remove(token.begin(), token.end(), '\"'), token.end());
    }
}