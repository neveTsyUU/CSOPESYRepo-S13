#include "Scheduler.h"
#include <chrono>
#include <iostream>


Scheduler::Scheduler(std::shared_ptr<Config> config, std::shared_ptr<ConsoleManager> mgr)
    : cpuCycles(0), config(config), manager(mgr)
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

void Scheduler::printProcessQueue() {
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

void Scheduler::reportUtilities(const std::string& fileName) const {
    std::ofstream fileUsed(fileName);
    if (!fileUsed.is_open()) {
        std::cerr << "Failed to create or open the file: " << fileName << "\n";
        return;
    }

    std::cout << "Writing screen -ls information to " << fileName << "!\n";

    uint32_t totalCores = static_cast<uint32_t>(schedulerCPUs.size());
    uint32_t usedCores = 0;

    for (const auto& cpu : schedulerCPUs) {
        if (!cpu->getIsFree()) {
            usedCores++;
        }
    }

    double utilization = (totalCores == 0) ? 0.0 : (static_cast<double>(usedCores) / totalCores) * 100.0;

    fileUsed << std::fixed << std::setprecision(2);
    fileUsed << "CPU Utilization: " << utilization << "%\n";
    fileUsed << "Cores used: " << usedCores << "\n";
    fileUsed << "Cores available: " << totalCores - usedCores << "\n";
    fileUsed << "\n--------------------------\n";

    fileUsed << "Running processes:\n";
    for (const auto& process : workingProcesses) {
        if (process->getAssignedCPUId() == -1) continue;

        fileUsed << std::left
            << std::setw(12) << process->getName()
            << "(" << process->getFormattedCreationTime() << ")   "
            << "Core: " << process->getAssignedCPUId() << "   "
            << std::left << std::setw(5) << process->getCurrentLine()
            << "/ " << std::left << std::setw(5) << process->getTotalLines()
            << "\n";
    }

    fileUsed << "\nFinished processes:\n";
    for (const auto& process : finishedProcesses) {
        fileUsed << std::left
            << std::setw(12) << process->getName()
            << "(" << process->getFormattedFinishTime() << ")   "
            << "Finished  "
            << std::left << std::setw(5) << process->getTotalLines()
            << "/ " << std::left << std::setw(5) << process->getTotalLines()
            << "\n";
    }

    fileUsed.close();
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
    return process;
}

void Scheduler::transferFinishedProcesses() {
    std::lock_guard<std::mutex> lock(processDispatchMutex);

    auto it = workingProcesses.begin();
    while (it != workingProcesses.end()) {
        if ((*it)->getState() == Process::State::Finished) {
            addToFinishedProcesses(*it);
            it = workingProcesses.erase(it);
        }
        else {
            ++it;
        }
    }
}

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
        cpuCycles++;

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
        if (nextProcess) {
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
        transferFinishedProcesses();
    }
}

void Scheduler::processMaker()
{
    uint32_t lastGenerationCycle = getCpuCycles();
    uint32_t generationFreq = config->getBatchProcessFreq();

    while (isMakingProcesses.load()) {
        uint32_t currentCPUCycle = getCpuCycles();
        if (currentCPUCycle - lastGenerationCycle >= generationFreq) {
            auto newProcess = createNextProcess();
            enqueueProcess(newProcess);
            lastGenerationCycle = currentCPUCycle;
        }

    }
}

std::vector<std::shared_ptr<Process>> Scheduler::getWorkingProcesses()
{
    return workingProcesses;
}

std::vector<std::shared_ptr<Process>> Scheduler::getFinishedProcesses()
{
    return finishedProcesses;
}

void Scheduler::startProcessMaker()
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

void Scheduler::stopProcessMaker()
{
    if (!isMakingProcesses.load()) {
        std::cout << "[Scheduler] Batch process generation is not currently running.\n\n";
        return;
    }

    isMakingProcesses.store(false);
    std::cout << "[Scheduler] Batch process generation stopped.\n\n";
}

uint32_t Scheduler::getCpuCycles() const {
    return cpuCycles.load();
}