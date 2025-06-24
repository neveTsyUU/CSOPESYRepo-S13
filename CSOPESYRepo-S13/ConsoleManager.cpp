#include "ConsoleManager.h"
#include <iostream>

void ConsoleManager::addConsole(const std::string& name, std::unique_ptr<ConsoleTemplate> console) {
    consoles[name] = std::move(console);
}

void ConsoleManager::setActiveConsole(const std::string& name) {
    auto it = consoles.find(name);
    if (it != consoles.end()) {
        system("cls");
        current = it->second.get();
        current->onEnabled();  // This can call display() + process()
    }
    else {
        std::cerr << "[ConsoleManager] Console \"" << name << "\" not found.\n";
    }
}

void ConsoleManager::run() {
    while (current != nullptr) {
        current->display();
        current->process();
    }
}

void ConsoleManager::mainPreInitializationHeader()
{
    std::cout << " ______     ______     ______     ______   ______     ______     __  __    \n";
    std::cout << "/\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   \n";
    std::cout << "\\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  __/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  \n";
    std::cout << " \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ \n";
    std::cout << "  \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ \n\n";
    std::cout << "Please initialize to continue. Type 'exit' to exit the emulator. \n";
}

void ConsoleManager::mainPreInitializationDisplayInput()
{
    std::cout << "[MainConsole]> ";
}

void ConsoleManager::initializationProcess()
{
    mainPreInitializationDisplayInput();
    std::string input;
    std::getline(std::cin, input);

    if (input == "in" || input == "initialize") {
        initialize();
        std::cout << "OS initialized.\n" << std::endl;
    }
    else if (input == "exit") {
        std::cout << "Exiting the program...\n";
        exit(0);
    }
    else
    {
        std::cout << "OS not initialized. Type 'in' or 'initialize' to initialize.\n" << std::endl;
    }
}

std::vector<std::string> ConsoleManager::listConsoleNames() const {
    std::vector<std::string> names;
    for (const auto& pair : consoles) {
        names.push_back(pair.first);
    }
    return names;
}

bool ConsoleManager::consoleExists(const std::string& name) const {
    return consoles.find(name) != consoles.end();
}

size_t ConsoleManager::getConsoleCount() const {
    return consoles.size();
}

void ConsoleManager::initialize()
{
    isInitialized = true;
}

bool ConsoleManager::initialized()
{
    return isInitialized;
}

ProcessConsole* ConsoleManager::getProcessConsole(const std::string& name)
{
    auto it = consoles.find(name);
    if (it != consoles.end()) {
        return dynamic_cast<ProcessConsole*>(it->second.get());
    }
    return nullptr;
}