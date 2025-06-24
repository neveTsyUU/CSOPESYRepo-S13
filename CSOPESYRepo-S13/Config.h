#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdint>

class Config {
private:
    uint32_t numCpu;
    std::string scheduler;
    uint32_t quantumCycles;
    uint32_t batchProcessFreq;
    uint32_t minIns;
    uint32_t maxIns;
    uint32_t delayPerExec;

public:
    void loadFromFile(const std::string& filename);
    void printConfig() const;

    uint32_t getNumCpu() const;
    const std::string& getScheduler() const;
    uint32_t getQuantumCycles() const;
    uint32_t getBatchProcessFreq() const;
    uint32_t getMinIns() const;
    uint32_t getMaxIns() const;
    uint32_t getDelayPerExec() const;
};

#endif // CONFIG_H