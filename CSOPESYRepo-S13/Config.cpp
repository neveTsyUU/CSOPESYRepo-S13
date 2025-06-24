#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

void Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::unordered_map<std::string, std::string> configMap;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (iss >> key >> value) {
            configMap[key] = value;
        }
    }

    numCpu = std::stoi(configMap["num-cpu"]);
    scheduler = configMap["scheduler"];
    if (scheduler.front() == '"' && scheduler.back() == '"') {
        scheduler = scheduler.substr(1, scheduler.size() - 2);
    }
    quantumCycles = std::stoi(configMap["quantum-cycles"]);
    batchProcessFreq = std::stoi(configMap["batch-process-freq"]);
    minIns = std::stoi(configMap["min-ins"]);
    maxIns = std::stoi(configMap["max-ins"]);
    delayPerExec = std::stoi(configMap["delay-per-exec"]);
}

void Config::printConfig() const {
    std::cout << "Number of CPUs: " << getNumCpu() << std::endl;
    std::cout << "Scheduler: " << getScheduler() << std::endl;
    std::cout << "Quantum Cycles: " << getQuantumCycles() << std::endl;
    std::cout << "Batch Process Frequency: " << getBatchProcessFreq() << std::endl;
    std::cout << "Min Instructions: " << getMinIns() << std::endl;
    std::cout << "Max Instructions: " << getMaxIns() << std::endl;
    std::cout << "Delay Per Exec: " << getDelayPerExec() << std::endl << std::endl;
}


uint32_t Config::getNumCpu() const {
    return numCpu;
}

const std::string& Config::getScheduler() const {
    return scheduler;
}

uint32_t Config::getQuantumCycles() const {
    return quantumCycles;
}

uint32_t Config::getBatchProcessFreq() const {
    return batchProcessFreq;
}

uint32_t Config::getMinIns() const {
    return minIns;
}

uint32_t Config::getMaxIns() const {
    return maxIns;
}

uint32_t Config::getDelayPerExec() const {
    return delayPerExec;
}