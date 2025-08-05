#include "Sleep.h"
#include "Process.h"

Sleep::Sleep(uint8_t duration)
    : sleepDuration(duration) {
}

void Sleep::execute(Process& process) {
    // Set the process to sleep for sleepDuration ticks
    process.setSleepTicksRemaining(sleepDuration);

    //process.log("Process sleeping for " + std::to_string(sleepDuration) + " ticks");
    //process.log("Process sleeping for " + std::to_string(sleepDuration) + " ticks. Progress: " + std::to_string(process.getCurrentLine()));

}

InstructionType Sleep::getType() const {
    return InstructionType::SLEEP;
}