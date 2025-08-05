#include "BackingStoreManager.h"
#include "Process.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

BackingStoreManager::BackingStoreManager() {
    std::ofstream storeFile("csopesy-backing-store.txt", std::ios::trunc);
    std::ofstream logFile("csopesy-backing-store-logs.txt", std::ios::trunc);
}

void BackingStoreManager::storeProcessToBackingStore(std::shared_ptr<Process> process)
{
    std::ofstream file("csopesy-backing-store.txt", std::ios::app);
    if (!file.is_open()) return;

    file << process->getId() << " "
        << process->getName() << " "
        << process->getCurrentLine() << " "
        << process->getTotalLines() << " "
        << process->getCreationTime() << " "
        << process->getProcessMemorySize() << " "
        << process->getProcessFrameSize() << " "
        << process->getNumberOfPages() << "\n";

    file.close();

    logBackingStoreOperation("STORE OPERATION: ", process);
}

void BackingStoreManager::loadProcessFromBackingStore(std::uint32_t id)
{
    std::ifstream file("csopesy-backing-store.txt");
    if (!file.is_open()) return;

    std::vector<std::string> lines;
    std::shared_ptr<Process> loadedProcess = nullptr;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::uint32_t pid, currentLine, totalLines, memSize, frameSize, numPages;
        std::string name;
        std::time_t creationTime;

        iss >> pid >> name >> currentLine >> totalLines >> creationTime >> memSize >> frameSize >> numPages;

        if (pid == id && loadedProcess == nullptr) {
            loadedProcess = std::make_shared<Process>(
                pid, name, currentLine, totalLines, creationTime, memSize, frameSize, numPages
            );
            // Don't push this line back (we're "removing" it)
        }
        else {
            lines.push_back(line);
        }
    }

    file.close();

    std::ofstream outFile("csopesy-backing-store.txt", std::ios::trunc);
    for (const auto& l : lines) {
        outFile << l << "\n";
    }
    outFile.close();

    if (loadedProcess) {
        logBackingStoreOperation("LOAD OPERATION: ", loadedProcess);
    }
}

void BackingStoreManager::logBackingStoreOperation(std::string operation, std::shared_ptr<Process> process)
{
    std::ofstream logFile("csopesy-backing-store-logs.txt", std::ios::app);
    if (!logFile.is_open()) return;

    logFile << operation
        << process->getId() << " "
        << process->getName() << " "
        << process->getCurrentLine() << " "
        << process->getTotalLines() << " "
        << process->getCreationTime() << " "
        << process->getProcessMemorySize() << " "
        << process->getProcessFrameSize() << " "
        << process->getNumberOfPages() << "\n";

    logFile.close();
}
