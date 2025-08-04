#include "ConsoleManager.h"
#include "MainConsole.h"
#include "Config.h"
#include "MarqueeConsole.h"
#include "Scheduler.h"
#include "PagingMemoryManager.h"
#include "BackingStoreManager.h"
#include "CPU.h"

//MCO2 V1

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
    auto backingStoreManager = std::make_shared<BackingStoreManager>();

    auto pagingManager = std::make_shared<PagingMemoryManager>(config->getMaxOverallMem(), config->getMemPerFrame());
 
    
    auto scheduler = std::make_shared<Scheduler>(config, manager, pagingManager, backingStoreManager);

    for (uint32_t i = 0; i < config->getNumCpu(); ++i) {
        cpuList.push_back(std::make_shared<CPU>(config, scheduler, i));
    }

    scheduler->setCPUList(cpuList);
    pagingManager->setSchedulerAccess(scheduler);

    auto marqueeConsole = std::make_unique<MarqueeConsole>("Marquee", manager);

    manager->addConsole("Main", std::make_unique<MainConsole>(manager, config, scheduler, pagingManager));
    manager->addConsole("Marquee", std::move(marqueeConsole));
    manager->setActiveConsole("Main");

    manager->run();
   
    return 0;
}
