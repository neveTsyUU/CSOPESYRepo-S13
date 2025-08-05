#include "Scheduler.h"
#include <chrono>
#include <iostream>

Scheduler::Scheduler(std::shared_ptr<Config> config, std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<PagingMemoryManager> pmgr, std::shared_ptr<BackingStoreManager> bsmngr)
    : cpuCycles(0), config(config), manager(mgr), pagingManager(pmgr), backingStoreManager(bsmngr)
{
    schedulerThread = std::thread(&Scheduler::schedulerLoop, this);
    schedulerThread.detach();
}

void Scheduler::setCPUList(std::vector<std::shared_ptr<CPU>> cpuList)
{
    schedulerCPUs = cpuList;
}

void Scheduler::printCPUList()
{
    {
        std::stringstream ss;
        ss << "[Scheduler] CPU List:\n";

        for (const auto& cpu : schedulerCPUs) {
            ss << " - CPU " << cpu->getCPUId()
                << " | Status: " << (cpu->getIsFree() ? "FREE" : "WORKING") << "\n";
        }

        std::cout << ss.str();
    }
}

void Scheduler::printProcessQueue()  {
    std::lock_guard<std::mutex> lock(processDispatchMutex);

    if (processQueue.empty()) {
        std::cout << "[Scheduler] Ready queue is empty.\n";
        return;
    }

    std::queue<std::shared_ptr<Process>> tempQueue = processQueue;
    std::ostringstream oss;
    oss << "[Scheduler] Ready queue: ";

    while (!tempQueue.empty()) {
        oss << tempQueue.front()->getName();
        tempQueue.pop();
        if (!tempQueue.empty()) oss << ", ";
    }

    std::cout << oss.str() << "\n";
}

void Scheduler::enqueueProcess(std::shared_ptr<Process> process)
{
    std::lock_guard<std::mutex> lock(processDispatchMutex); 
    processQueue.push(process);
}

void Scheduler::printWorkingAndFinishedProcesses() {
    std::lock_guard<std::mutex> lock(processDispatchMutex);
    std::stringstream ss;

    // --- CPU Utilization ---
    uint32_t totalCores = static_cast<uint32_t>(schedulerCPUs.size());
    uint32_t usedCores = 0;

    for (const auto& cpu : schedulerCPUs) {
        if (!cpu->getIsFree()) {
            usedCores++;
        }
    }

    double utilization = (totalCores == 0) ? 0.0 : (static_cast<double>(usedCores) / totalCores) * 100.0;

    ss << std::fixed << std::setprecision(2);
    ss << "CPU Utilization: " << utilization << "%\n";
    ss << "Cores used: " << usedCores << "\n";
    ss << "Cores available: " << totalCores - usedCores << "\n";

    ss << "\n--------------------------\n";

    // --- Running Processes ---
    ss << "Running processes:\n";
    for (const auto& process : workingProcesses) {
        if (process->getAssignedCPUId() == -1) {
            continue;
        }

        ss << std::left
            << std::setw(12) << process->getName()
            << "(" << process->getFormattedCreationTime() << ")   "
            << "Core: " << process->getAssignedCPUId() << "   "
            << std::left << std::setw(5) << process->getCurrentLine()
            << "/ " << std::left << std::setw(5) << process->getTotalLines()
            << "\n";
    }

    // --- Finished Processes ---
    ss << "\nFinished processes:\n";
    for (const auto& process : finishedProcesses) {
        ss << std::left
            << std::setw(12) << process->getName()
            << "(" << process->getFormattedFinishTime() << ")   "
            << "Finished  "
            << std::left << std::setw(5) << process->getTotalLines()
            << "/ " << std::left << std::setw(5) << process->getTotalLines()
            << "\n";
    }

    std::cout << ss.str();
}


std::shared_ptr<Process> Scheduler::createNextProcess()
{
    uint32_t processNumber = processID++;
    std::string processName = "process" + std::to_string(processNumber);
    auto newProcess = std::make_shared<Process>(processNumber, processName, config);
    manager->addConsole(processName, std::make_unique<ProcessConsole>(manager, processName, newProcess));
    return newProcess;
}

void Scheduler::addToWorkingProcesses(std::shared_ptr<Process> process) {
    workingProcesses.push_back(process);
}

void Scheduler::addToFinishedProcesses(std::shared_ptr<Process> process) {
    finishedProcesses.push_back(process);
}

bool Scheduler::isProcessQueueEmpty()
{
    return processQueue.empty();
}

std::shared_ptr<Process> Scheduler::dequeueProcess()
{
    auto process = processQueue.front();
    processQueue.pop();

    // p1 p2 p3
    // p2 p3 p1 (enqueue)
    return process;
}

//void Scheduler::transferFinishedProcesses() {
//    std::lock_guard<std::mutex> lock(processDispatchMutex);
//
//    auto it = workingProcesses.begin();
//    while (it != workingProcesses.end()) {
//        if ((*it)->getState() == Process::State::Finished) {
//            addToFinishedProcesses(*it);
//            it = workingProcesses.erase(it);
//        }
//        else {
//            ++it;
//        }
//    }
//}

std::shared_ptr<CPU> Scheduler::getRandomFreeCPU()
{
    std::vector<std::shared_ptr<CPU>> freeCPUs;
    for (const auto& cpu : schedulerCPUs) {
        if (cpu->getIsFree()) {
            freeCPUs.push_back(cpu);
        }
    }

    if (freeCPUs.empty()) {
        return nullptr; 
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(freeCPUs.size() - 1));

    return freeCPUs[dist(gen)];
}

void Scheduler::schedulerLoop() {
    while (true) {
        transferFinishedProcesses();
        cpuCycles++;

        uint32_t activeCPUs = getNumUsedCPU();

        if (activeCPUs > 0) {
            activeCycles++;
        }
        else if (activeCPUs == 0) {
            idleCycles++;
        }

        auto freeCPU = getRandomFreeCPU();
        std::shared_ptr<Process> nextProcess = nullptr;  

        {
            std::lock_guard<std::mutex> lock(processDispatchMutex);
            if (freeCPU) {
                if (!isProcessQueueEmpty()) {
                    nextProcess = dequeueProcess();
                }
            }
        }
        if (freeCPU && nextProcess) {
            if (!pagingManager->isProcessAllocated(nextProcess)) {
         /*       std::cout << "[DEBUG] Scheduler loop running...\n";*/
                {
                    std::lock_guard<std::mutex> lock(backingStoreOperationMutex);
                    backingStoreManager->loadProcessFromBackingStore(nextProcess->getId());
                }

                bool allocable = pagingManager->allocateMemoryForProcess(nextProcess);

                if (allocable) {
                    if (nextProcess->getState() == Process::State::Ready) {
                        freeCPU->assignProcess(nextProcess);
                        freeCPU->run();
                        if (nextProcess->getState() == Process::State::Running) {
                            addToWorkingProcesses(nextProcess);
                        }
                    }
                    else if (nextProcess->getState() == Process::State::Waiting) {
                        freeCPU->assignProcess(nextProcess);
                        freeCPU->run();
                    }
                }
                if (!allocable) {
                    auto processToBeEvicted = pagingManager->getOldestProcess();

                    if (!processToBeEvicted) {
                        enqueueProcess(nextProcess);
                        continue;
                    }

                    pagingManager->deallocateMemoryUsedByProcess(processToBeEvicted->getId());

                    {
                        std::lock_guard<std::mutex> lock(backingStoreOperationMutex);
                        backingStoreManager->storeProcessToBackingStore(processToBeEvicted);
                    }

                    bool allocable2 = pagingManager->allocateMemoryForProcess(nextProcess);

                    if (allocable2) {
                        if (nextProcess->getState() == Process::State::Ready) {
                            freeCPU->assignProcess(nextProcess);
                            freeCPU->run();
                            if (nextProcess->getState() == Process::State::Running) {
                                addToWorkingProcesses(nextProcess);
                            }
                        }
                        else if (nextProcess->getState() == Process::State::Waiting) {
                            freeCPU->assignProcess(nextProcess);
                            freeCPU->run();
                        }
                    }
                }
            }
            else {
                if (nextProcess->getState() == Process::State::Ready) {
                    freeCPU->assignProcess(nextProcess);
                    freeCPU->run();
                    if (nextProcess->getState() == Process::State::Running) {
                        addToWorkingProcesses(nextProcess);
                    }
                }
                else if (nextProcess->getState() == Process::State::Waiting) {
                    freeCPU->assignProcess(nextProcess);
                    freeCPU->run();
                }
            }
        }
        //std::cout << "here" << std::endl << std::endl;
        /*transferFinishedProcesses();*/
        std::this_thread::sleep_for(std::chrono::microseconds(10000)); 
    }
}

void Scheduler::transferFinishedProcesses() {
    std::lock_guard<std::mutex> lock(processDispatchMutex);

    auto it = workingProcesses.begin();
    while (it != workingProcesses.end()) {
        if ((*it)->getState() == Process::State::Finished) {
            // Deallocate memory for the finished process
            pagingManager->deallocateMemoryUsedByProcess((*it)->getId());

            // Move to finished list
            addToFinishedProcesses(*it);

            // Remove from working list
            it = workingProcesses.erase(it);
        }
        else {
            ++it;
        }
    }
}

//void Scheduler::schedulerLoop() {
//    while (true) {
//        cpuCycles++;
//
//        uint32_t activeCPUs = getNumUsedCPU();
//
//        if (activeCPUs > 0) {
//            activeCycles++;
//        }
//        else if (activeCPUs == 0) {
//            idleCycles++;
//        }
//
//        auto freeCPU = getRandomFreeCPU();
//        std::shared_ptr<Process> nextProcess = nullptr;
//
//        {
//            std::lock_guard<std::mutex> lock(processDispatchMutex);
//            if (freeCPU) {
//                if (!isProcessQueueEmpty()) {
//                    nextProcess = dequeueProcess();
//                }
//            }
//        }
//        if (nextProcess) {
//            if (nextProcess->getState() == Process::State::Ready) {
//                freeCPU->assignProcess(nextProcess);
//                freeCPU->run();
//
//                if (nextProcess->getState() == Process::State::Running) {
//                    addToWorkingProcesses(nextProcess);
//                }
//            }
//            else if (nextProcess->getState() == Process::State::Waiting) {
//                freeCPU->assignProcess(nextProcess);
//                freeCPU->run();
//
//                if (nextProcess->getState() == Process::State::Running) {
//                    addToWorkingProcesses(nextProcess);
//                }
//            }
//        }
//        transferFinishedProcesses();
//        /*    std::this_thread::sleep_for(std::chrono::microseconds(100000)); */
//    }
//}

void Scheduler::processMaker()
{
    uint32_t lastGenerationCycle = getCpuCycles();
    uint32_t generationFreq = config->getBatchProcessFreq();  

    while (isMakingProcesses.load()) { // false
        uint32_t currentCPUCycle = getCpuCycles();                     // 1 yung batchprocessfreq
        if (currentCPUCycle - lastGenerationCycle >= generationFreq) { // 1 CREATE PROCESS 2 CREATE PROCES
                                                                       // 2
                                                                       // 1 CREATE PROCESS 3 CREATE PROCESS 5 CREATE PROCESS
                                                                       // 3
                                                                       // 1 CREATE PROCESS 4 CREATE PROCESS 7 CREATE PROCESS
            auto newProcess = createNextProcess();
            enqueueProcess(newProcess);
            lastGenerationCycle = currentCPUCycle;
        }

    }
    // end
}

    //else if (input == "scheduler-start" || input == "st") {
    //scheduler->startProcessMaker();
    //}
    //else if (input == "scheduler-stop" || input == "ss") {
    //    scheduler->stopProcessMaker();
    //    }

void Scheduler::startProcessMaker() // READ ME
{
    if (isMakingProcesses.load()) {
        std::cout << "[Scheduler] Batch process generation is already running.\n\n";
        return;
    }

    isMakingProcesses.store(true);
    processMakerThread = std::thread(&Scheduler::processMaker, this);
    processMakerThread.detach();

    std::cout << "[Scheduler] Batch process generation started.\n\n";
}

void Scheduler::stopProcessMaker() // READ ME
{
    if (!isMakingProcesses.load()) {
        std::cout << "[Scheduler] Batch process generation is not currently running.\n\n";
        return;
    }

    isMakingProcesses.store(false);
    std::cout << "[Scheduler] Batch process generation stopped.\n\n";
}

std::vector<std::shared_ptr<Process>> Scheduler::getWorkingProcesses()
{
    return workingProcesses;
}

std::vector<std::shared_ptr<Process>> Scheduler::getFinishedProcesses()
{
    return finishedProcesses;
}


uint32_t Scheduler::getCpuCycles() const {
    return cpuCycles.load();
}

uint32_t Scheduler::getActiveCycles() const
{
    return activeCycles.load();
}

uint32_t Scheduler::getIdleCycles() const
{
    return idleCycles.load();
}

std::uint32_t Scheduler::getNumUsedCPU()
{
    uint32_t usedCores = 0;
    for (const auto& cpu : schedulerCPUs) {
        if (!cpu->getIsFree()) {
            usedCores++;
        }
    }
    return usedCores;
}

std::uint32_t Scheduler::getNumCPU()
{
    return schedulerCPUs.size();
}
