#include "MainConsole.h"
#include "ConsoleManager.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string>

MainConsole::MainConsole(std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched)
    : manager(mgr), config(cnfg), scheduler(sched) {
}

void MainConsole::onEnabled() {
    system("cls");
    std::cout << " ______     ______     ______     ______   ______     ______     __  __    \n";
    std::cout << "/\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   \n";
    std::cout << "\\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  __/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  \n";
    std::cout << " \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ \n";
    std::cout << "  \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ \n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    std::cout << "Welcome to the CSOPESY command line interface (CLI)!\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen and 'list' for a list of commands.\n";
}

void MainConsole::display() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    std::cout << "[MainConsole]> ";
}

void MainConsole::process() {
    std::string input;
    std::getline(std::cin >> std::ws, input);
    handleCommand(input);
}

void MainConsole::handleCommand(const std::string& input) {
    if (input == "exit") {
        std::cout << "Exiting the program...\n";
        exit(0);
    }
    else if (input == "clear") {
        system("cls");
        onEnabled();
    }
    else if (input == "cpu-cycles") {
        std::cout << "CPU Cycles: " << scheduler->getCpuCycles() << std::endl << std::endl;
    }
    else if (input == "marquee") {
        manager->setActiveConsole("Marquee");
    }
    else if (input == "scheduler-start" || input == "st") {
        scheduler->startProcessMaker();
    }
    else if (input == "scheduler-stop" || input == "ss") {
        scheduler->stopProcessMaker();
    }
    else if (input == "screen -ls" || input == "e") {
        scheduler->printWorkingAndFinishedProcesses();
    }
    else if (input == "report-util") {
        scheduler->reportUtilities("csopesy-log.txt");
    }
    else if (input == "list") {
        std::cout << "*=========================== COMMANDS ==============================*\n";
        std::cout << "| 'exit'             -> Exit the program                            |\n";
        std::cout << "| 'clear'            -> Clear the screen                            |\n";
        std::cout << "| 'list'             -> List available commands                     |\n";
        std::cout << "| 'count'            -> Show total registered consoles              |\n";
        std::cout << "| 'screen -s X'      -> Creates a new screen with name X            |\n";
        std::cout << "| 'screen -r X'      -> Resume existing screen with name X          |\n";
        std::cout << "| 'scheduler-start'  -> Creates a bunch of dummy processes          |\n";
        std::cout << "| 'scheduler-stop'   -> Stops the production of dummy processes     |\n";
        std::cout << "| 'screen -ls'       -> Shows CPU utilization and scheduler report  |\n";
        std::cout << "| 'report-util'      -> Performs screen -ls but on a text file      |\n";
        std::cout << "*===================================================================*\n";
        std::cout << std::endl;
    }
    else if (input == "count") {
        std::cout << "Total consoles: " << manager->getConsoleCount() << "\n";
        auto names = manager->listConsoleNames();
        for (const auto& name : names) {
            std::cout << "- " << name << "\n";
        }
        std::cout << std::endl;
    }
    else if (input.rfind("screen -r ", 0) == 0) {
        std::string screenName = input.substr(10);
        if (!screenName.empty()) {
            if (screenName != "Main" && screenName != "Marquee" && screenName != "main" && screenName != "marquee") {
                if (manager->consoleExists(screenName)) {
                    ProcessConsole* pc = manager->getProcessConsole(screenName);
                    if (pc->isProcessFinished()) {
                        std::cout << "Cannot enter: process finished" << "\n\N";
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
                if (screenName == "Main" || screenName == "main") {
                    std::cout << "You are already on the main screen.\n";
                    std::cout << std::endl;
                }
                else if (screenName == "Marquee" || screenName == "marquee") {
                    std::cout << "Type 'marquee' to go to the marquee screen.\n";
                    std::cout << std::endl;
                }
            }
        }
        else {
            std::cout << "Please specify a screen name.\n";
            std::cout << std::endl;
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
    else {
        std::cout << "Unknown command. Type 'list' to see available options.\n";
        std::cout << std::endl;
    }
}
