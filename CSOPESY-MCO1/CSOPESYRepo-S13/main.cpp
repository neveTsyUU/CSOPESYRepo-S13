#include <string>
#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "ConsoleManager.h"
#include "MainConsole.h"
#include "Config.h"
#include "MarqueeConsole.h"
#include "Scheduler.h"
#include "CPU.h"

int main() {

    auto manager = std::make_shared<ConsoleManager>();

    manager->mainPreInitializationHeader();

    while (!manager->initialized()) {
        manager->initializationProcess();
    }

    system("cls");
    auto config = std::make_shared<Config>();
    config->loadFromFile("config.txt");

    std::vector<std::shared_ptr<CPU>> cpuList;

    auto scheduler = std::make_shared<Scheduler>(config, manager);

    for (uint32_t i = 0; i < config->getNumCpu(); ++i) {
        cpuList.push_back(std::make_shared<CPU>(config, scheduler, i));
    }

    scheduler->setCPUList(cpuList);

    auto marqueeConsole = std::make_unique<MarqueeConsole>("Marquee", manager);



    manager->addConsole("Main", std::make_unique<MainConsole>(manager, config, scheduler));
    manager->addConsole("Marquee", std::move(marqueeConsole));
    manager->setActiveConsole("Main");


    // Start the console loop
    //sampCPU->run();
    manager->run();



    return 0;
}