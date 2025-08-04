#include "PagingMemoryManager.h"
#include "Scheduler.h"
#include <iostream>
#include <algorithm>

PagingMemoryManager::PagingMemoryManager(uint32_t maxMemory, uint32_t memoryFrame)
{
    maxOverAllMemory = maxMemory;
    memoryPerFrame = memoryFrame;
    nPages = maxOverAllMemory / memoryPerFrame;
    memoryMap.resize(nPages, -1); // -1 indicates free frame
}

bool PagingMemoryManager::allocateMemoryForProcess(std::shared_ptr<Process> process)
{
    std::uint32_t requiredPages = process->getNumberOfPages();
    std::uint32_t pid = process->getId();

    // Collect free frame indices
    std::vector<std::uint32_t> freeIndices;
    for (std::uint32_t i = 0; i < memoryMap.size(); ++i) {
        if (memoryMap[i] == -1) {
            freeIndices.push_back(i);
            if (freeIndices.size() == requiredPages)
                break;
        }
    }

    // Not enough memory to allocate
    if (freeIndices.size() < requiredPages)
        //std::cout << "Not enough memory to allocate process " << pid << "\n";
        return false;

    // Assign frames to process
    for (std::uint32_t idx : freeIndices) {
        memoryMap[idx] = pid;
        ++numPagedIn; // ✅ track pages allocated
    }

    // Update actual memory used (not full page size)
    usedMemory += process->getProcessMemorySize();
    processesInMemory.push_back(process);
    return true;
}

void PagingMemoryManager::deallocateMemoryUsedByProcess(std::uint32_t pid)
{
    // Find the process to get its memorySize
    std::shared_ptr<Process> foundProcess = nullptr;
    for (auto& p : processesInMemory) {
        if (p->getId() == pid) {
            foundProcess = p;
            break;
        }
    }

    if (foundProcess) {
        usedMemory -= foundProcess->getProcessMemorySize();
    }

    // Free frames belonging to the process
    for (std::uint32_t& frame : memoryMap) {
        if (frame == pid) {
            frame = -1;
            ++numPagedOut; // ✅ track pages freed
        }
    }

    // Remove process from vector
    processesInMemory.erase(
        std::remove_if(processesInMemory.begin(), processesInMemory.end(),
            [pid](std::shared_ptr<Process> p) {
                return p->getId() == pid;
            }),
        processesInMemory.end()
    );
}


void PagingMemoryManager::visualizeMemory()
{
    std::cout << "\n--- Memory Visualization ---\n";
    for (std::size_t i = 0; i < memoryMap.size(); ++i) {
        std::cout << "Frame [" << i << "]: ";
        if (memoryMap[i] == -1)
            std::cout << "Free\n";
        else
            std::cout << "PID " << memoryMap[i] << "\n";
    }

    double usagePercent = (static_cast<double>(usedMemory) / maxOverAllMemory) * 100.0;
    std::cout << "Used Memory: " << usedMemory << " / " << maxOverAllMemory
        << " bytes (" << usagePercent << "%)\n";
}

std::uint32_t PagingMemoryManager::getMemoryPerFrame()
{
    return memoryPerFrame;
}

void PagingMemoryManager::setSchedulerAccess(std::shared_ptr<Scheduler> sched)
{
    scheduler = sched;
}

void PagingMemoryManager::printSmthFromScheduler()
{
    std::cout << scheduler->getCpuCycles() << std::endl << std::endl;
}

bool PagingMemoryManager::isProcessAllocated(std::shared_ptr<Process> process)
{
    std::uint32_t pid = process->getId();

    for (std::uint32_t frame : memoryMap) {
        if (frame == pid)
            return true;
    }

    return false;
}

std::shared_ptr<Process> PagingMemoryManager::getOldestProcess()
{
    std::shared_ptr<Process> oldest = nullptr;

    for (const auto& proc : processesInMemory)
    {
        if (proc->getState() == Process::State::Waiting)
        {
            if (!oldest || proc->getId() < oldest->getId())
            {
                oldest = proc;
            }
        }
    }

    return oldest;
}

void PagingMemoryManager::vmStat()
{
    std::ostringstream ss;

    ss << "-------------------------------------------\n";
    ss << "|   VMSTAT V01.00 Driver Version: 01.00   |\n";
    ss << "-------------------------------------------\n";
    ss << "Total Memory: " << maxOverAllMemory << "B\n";
    ss << "Used Memory: " << usedMemory << "B\n";
    ss << "Free Memory: " << (maxOverAllMemory - usedMemory) << "B\n";
    ss << "Idle CPU Ticks: " << scheduler->getIdleCycles() << "\n";
    ss << "Active CPU Ticks: " << scheduler->getActiveCycles() << "\n";
    ss << "Total CPU Ticks: " << scheduler->getCpuCycles() << "\n";
    ss << "Num Paged In: " << numPagedIn << "\n";
    ss << "Num Paged Out: " << numPagedOut << "\n";
    ss << "-------------------------------------------\n\n";

    std::cout << ss.str();
}

void PagingMemoryManager::processSMI()
{
    std::ostringstream ss;

    float cpuUtil = 0.0f;
    if (scheduler->getNumCPU() > 0) {
        cpuUtil = (static_cast<float>(scheduler->getNumUsedCPU()) /
            static_cast<float>(scheduler->getNumCPU())) * 100.0f;
    }

    float memUtil = (static_cast<float>(usedMemory) / static_cast<float>(maxOverAllMemory)) * 100.0f;

    ss << "-------------------------------------------\n";
    ss << "| PROCESS-SMI V01.00 Driver Version: 01.00|\n";
    ss << "-------------------------------------------\n";
    ss << std::fixed << std::setprecision(2);
    ss << "CPU-Util: " << cpuUtil << "%\n";
    ss << "Memory Usage: " << usedMemory << "B / " << maxOverAllMemory << "B\n";
    ss << "Memory Util: " << memUtil << "%\n";

    ss << "\n===========================================\n";
    ss << "Running processes and memory usage:\n";
    ss << "-------------------------------------------\n";

    for (const auto& process : processesInMemory) {
        ss << process->getName() << " " << process->getProcessMemorySize() << "B\n";
    }

    ss << "-------------------------------------------\n\n";

    std::cout << ss.str();
}



