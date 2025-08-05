#pragma once
#include "Process.h"
class Scheduler; 

class PagingMemoryManager
{
public:
	PagingMemoryManager(uint32_t maxMemory, uint32_t memoryFrame);
	bool allocateMemoryForProcess(std::shared_ptr<Process> process);
	void deallocateMemoryUsedByProcess(std::uint32_t pid);
	void visualizeMemory();
	std::uint32_t getMemoryPerFrame();
	void setSchedulerAccess(std::shared_ptr<Scheduler> sched);
	void printSmthFromScheduler();
	bool isProcessAllocated(std::shared_ptr<Process> process);
	std::shared_ptr<Process> getOldestProcess();
	void vmStat();
	void processSMI();

private:
	std::vector<std::shared_ptr<Process>> processesInMemory;
	std::vector<std::uint32_t> memoryMap;
	std::shared_ptr<Scheduler> scheduler;

	uint32_t maxOverAllMemory;
	uint32_t memoryPerFrame;
	uint32_t nPages;
	uint32_t numPagedIn = 0;
	uint32_t numPagedOut = 0;
	uint32_t usedMemory = 0;
};

