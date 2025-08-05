#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <iomanip> // for std::put_time
#include <ctime>
#include "Config.h"
#include "CPU.h"
#include "Process.h"
#include "ProcessConsole.h"
#include "ConsoleManager.h"
#include "PagingMemoryManager.h"
#include "BackingStoreManager.h"
#include <sstream> 
#include <random>
#include <queue>

class Scheduler {
private:
    std::atomic_uint32_t cpuCycles;
    std::atomic_uint32_t activeCycles;
    std::atomic_uint32_t idleCycles;
    std::thread schedulerThread;
    std::thread processMakerThread;
    std::shared_ptr<Config> config;
    std::shared_ptr<ConsoleManager> manager;
    std::shared_ptr<PagingMemoryManager> pagingManager;
    std::shared_ptr<BackingStoreManager> backingStoreManager;
    std::queue<std::shared_ptr<Process>> processQueue;
    std::vector<std::shared_ptr<Process>> workingProcesses;
    std::vector<std::shared_ptr<Process>> finishedProcesses;
    std::vector<std::shared_ptr<CPU>> schedulerCPUs;
    bool isProcessQueueEmpty();
    std::shared_ptr<Process> dequeueProcess();
    void transferFinishedProcesses();

    std::shared_ptr<CPU> getRandomFreeCPU();

    void addToWorkingProcesses(std::shared_ptr<Process> process);
    void addToFinishedProcesses(std::shared_ptr<Process> process);

    void schedulerLoop();
    void processMaker();
    std::vector<std::shared_ptr<Process>> getWorkingProcesses();
    std::vector<std::shared_ptr<Process>> getFinishedProcesses();

    std::mutex processDispatchMutex;
    std::mutex backingStoreOperationMutex;
    std::atomic<bool> isMakingProcesses{ false };
    std::uint32_t processID = 0;
    std::string fileName;

public:
    Scheduler(std::shared_ptr<Config> cfg, std::shared_ptr<ConsoleManager> mgr, std::shared_ptr<PagingMemoryManager> pmgr, std::shared_ptr<BackingStoreManager> bsmngr);
    void setCPUList(std::vector<std::shared_ptr<CPU>> cpuList);
    void printCPUList();
    void printProcessQueue();
    void startProcessMaker();
    void stopProcessMaker();
    void enqueueProcess(std::shared_ptr<Process> process);
    void printWorkingAndFinishedProcesses();
    void reportUtilities(const std::string& fileName) const;
    std::shared_ptr<Process> createNextProcess();
    uint32_t getCpuCycles() const;
    uint32_t getActiveCycles() const;
    uint32_t getIdleCycles() const;
    std::uint32_t getNumUsedCPU();
    std::uint32_t getNumCPU();
};
