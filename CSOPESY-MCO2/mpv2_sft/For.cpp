#include "For.h"
#include "Print.h"
#include "Declare.h"
#include "Add.h"
#include "Subtract.h"
#include "Sleep.h"
#include "Process.h"
#include <random>
#include <cstdlib>

For::For(uint32_t iterations)
    : repeatCount(iterations) {
    createRandomSetOfCommands();
}

void For::createRandomSetOfCommands() {
    int low = 1;
    int high = 10;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(low, high);

    int numInstructions = dist(gen);
    miniInstructionList.clear();

    for (int i = 0; i < numInstructions; ++i) {
        int instructionType = std::rand() % 14; // 0 to 13 only

        if (instructionType == 0) {
            uint32_t nestedCount = (std::rand() % 4) + 1;
            miniInstructionList.push_back(std::make_shared<For>(nestedCount));
        }
        else if (instructionType == 1) {
            miniInstructionList.push_back(std::make_shared<Print>("[FOR] This is a message"));
            //instructionName.push_back("PRINT");
        }
        else if (instructionType == 2) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            std::string msg = "[FOR] Displaying value of variable: " + std::string(1, varName) + ": ";
            miniInstructionList.push_back(std::make_shared<Print>(msg, value));
            //instructionName.push_back("PRINT");
        }
        else if (instructionType == 3) {
            char varName = getRandomVariableName();
            miniInstructionList.push_back(std::make_shared<Declare>(std::string(1, varName)));
            //instructionName.push_back("DECLARE");
        }
        else if (instructionType == 4) {
            char varName = getRandomVariableName();
            uint16_t value = getRandomUint16();
            miniInstructionList.push_back(std::make_shared<Declare>(std::string(1, varName), value));
            //instructionName.push_back("DECLARE");
        }
        else if (instructionType == 5) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            miniInstructionList.push_back(std::make_shared<Add>(std::string(1, dest), op1, op2));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 6) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            miniInstructionList.push_back(std::make_shared<Add>(std::string(1, dest), op1));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 7) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            miniInstructionList.push_back(std::make_shared<Add>(std::string(1, dest), n, op2));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 8) {
            // ADD(dest)
            char dest = getRandomVariableName();
            miniInstructionList.push_back(std::make_shared<Add>(std::string(1, dest)));
            //instructionName.push_back("ADD");
        }
        else if (instructionType == 9) {
            // ADD(dest, op1, op2)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            miniInstructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), op1, op2));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 10) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op1 = static_cast<int16_t>(getRandomUint16());
            miniInstructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), op1));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 11) {
            // ADD(dest, op1)
            char dest = getRandomVariableName();
            int16_t op2 = static_cast<int16_t>(getRandomUint16());
            std::nullopt_t n = std::nullopt;
            miniInstructionList.push_back(std::make_shared<Subtract>(std::string(1, dest), n, op2));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 12) {
            // ADD(dest)
            char dest = getRandomVariableName();
            miniInstructionList.push_back(std::make_shared<Subtract>(std::string(1, dest)));
            //instructionName.push_back("SUB");
        }
        else if (instructionType == 13) {
            // ADD(dest)
            uint8_t sleepTime = getRandomUint8();
            miniInstructionList.push_back(std::make_shared<Sleep>(sleepTime));
            //instructionName.push_back("SLEEP");
        }
    }
}

void For::execute(Process& process) {
    //process.log("For loop has " + std::to_string(miniInstructionList.size()) + " commands that will repeat " + std::to_string(repeatCount) + " times!");
    for (uint32_t i = 0; i < repeatCount; ++i) {
        //process.log(">>> [For] Starting iteration " + std::to_string(i + 1));
        for (const auto& cmd : miniInstructionList) {
            cmd->execute(process);
        }
    }
    //process.log(">>> For loop done.");

}

char For::getRandomVariableName() {
    static const std::string varNames = "abcde";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<std::size_t> dist(0, varNames.size() - 1);
    return varNames[dist(rng)];
}

uint16_t For::getRandomUint16() {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<uint16_t> dist(0, UINT16_MAX);
    return dist(rng);
}

uint8_t For::getRandomUint8() {
    return 8;
}

InstructionType For::getType() const {
    return InstructionType::FOR;
}