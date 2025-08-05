#pragma once
#ifndef CPU_H
#define CPU_H

#include <thread>
#include <atomic>
#include <memory>
#include "Process.h"
class Scheduler;


class CPU {
private:
    std::atomic_bool isFree;
    std::shared_ptr<Process> assignedProcess;
    std::thread worker;
    std::shared_ptr<Config> config;
    std::shared_ptr<Scheduler> scheduler;
    std::uint32_t cpuId;

public:
    CPU(std::shared_ptr<Config> cnfg, std::shared_ptr<Scheduler> sched, std::uint32_t cpu);

    void assignProcess(std::shared_ptr<Process> process);
    bool getIsFree() const;
    void run();
    void FCFSCPU();
    void RRCPU();
    void start();
    void stop();
    std::uint32_t getCPUId();

    std::shared_ptr<Process> getAssignedProcess() const;
};

#endif // CPU_H

