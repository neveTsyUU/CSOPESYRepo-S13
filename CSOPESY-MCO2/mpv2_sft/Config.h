// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <cstdint>

class Config {
private:
    uint32_t numCpu = 0;
    std::string scheduler;
    uint32_t quantumCycles = 0;
    uint32_t batchProcessFreq = 0;
    uint32_t minIns = 0;
    uint32_t maxIns = 0;
    uint32_t delayPerExec = 0;

    uint32_t maxOverallMem = 0;
    uint32_t memPerFrame = 0;
    uint32_t minMemPerProc = 0;
    uint32_t maxMemPerProc = 0;

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

    uint32_t getMaxOverallMem() const;
    uint32_t getMemPerFrame() const;
    uint32_t getMinMemPerProc() const;
    uint32_t getMaxMemPerProc() const;
};

#endif // CONFIG_H
