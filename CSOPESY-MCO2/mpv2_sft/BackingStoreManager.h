#pragma once
#include <cstdint>
#include <memory>
#include <string>

class Process;

class BackingStoreManager
{
public:
	BackingStoreManager();
	void storeProcessToBackingStore(std::shared_ptr<Process> process);
	void loadProcessFromBackingStore(std::uint32_t id);
	void logBackingStoreOperation(std::string backingStoreOperation,std::shared_ptr<Process> process);
};

