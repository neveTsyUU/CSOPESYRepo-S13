#include "CPU.h"
#include "Scheduler.h"  

CPU::CPU(std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched, std::uint32_t cpu) : isFree(true), assignedProcess(nullptr), config(cnfg), scheduler(sched), cpuId(cpu) {}

void CPU::assignProcess(std::shared_ptr<Process> process) {
    assignedProcess = process;
}

void CPU::start() {
    isFree = false;
}

void CPU::stop() {
    isFree = true;
}

std::uint32_t CPU::getCPUId()
{
    return cpuId;
}

bool CPU::getIsFree() const {
    return isFree;
}

void CPU::run()
{
    start();
    assignedProcess->assignCpuId(this->getCPUId());
    assignedProcess->setState(Process::State::Running);
    std::string algorithm = config->getScheduler();
    if (algorithm == "fcfs") {
        worker = std::thread(&CPU::FCFSCPU, this);
        worker.detach();
    }
    else if (algorithm == "rr") {
        worker = std::thread(&CPU::RRCPU, this);
        worker.detach();
    }
}

void CPU::FCFSCPU()
{
    uint32_t currentCycles = scheduler->getCpuCycles();
    uint32_t nextCycle = currentCycles + config->getDelayPerExec() + 1;

    while (!assignedProcess->isFinished()) {
        while (scheduler->getCpuCycles() < nextCycle) {
            // Simulate delay
        }

        if (assignedProcess->executeCurrentLine()) {
            assignedProcess->advanceLine();  // Only if not sleeping
        }

        nextCycle += config->getDelayPerExec() + 1;
    }

    assignedProcess->markFinished();
    stop();
}

void CPU::RRCPU()
{
    uint32_t currentCycles = scheduler->getCpuCycles();
    uint32_t nextCycle = currentCycles + config->getDelayPerExec() + 1;
    uint32_t quantumCycles = config->getQuantumCycles();
    uint32_t time_slice = 0; // time process spent on the CPU

    while (!assignedProcess->isFinished() && time_slice < quantumCycles) {
        while (scheduler->getCpuCycles() < nextCycle) {
            // Simulate delay
        }

        if (assignedProcess->executeCurrentLine()) {
            assignedProcess->advanceLine();  // Only if not sleeping
        }

        time_slice++;

        nextCycle += config->getDelayPerExec() + 1;
    }

    if (!assignedProcess->isFinished()) {
        assignedProcess->assignCpuId(-1);
        assignedProcess->setState(Process::State::Waiting);
        scheduler->enqueueProcess(assignedProcess);
    }
    else {
        assignedProcess->markFinished();
    }

    stop();
}


std::shared_ptr<Process> CPU::getAssignedProcess() const {
    return assignedProcess;
}