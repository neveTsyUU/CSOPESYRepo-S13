#include "MarqueeConsole.h"
#include "ProcessConsole.h"
#include "ConsoleManager.h"
#include "ConsoleTemplate.h"
#include <thread>
#include <atomic>
#include <string>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>
#include <mutex>


MarqueeConsole::MarqueeConsole(const std::string& name, std::shared_ptr<ConsoleManager> mgr)
    : ConsoleTemplate(), manager(mgr), running(false) {
}

MarqueeConsole::~MarqueeConsole() {
    if (bounceThread.joinable()) bounceThread.join();
    if (inputThread.joinable()) inputThread.join();

    input.clear();
    commands.clear();
}

void MarqueeConsole::onEnabled()
{
    process();
}

std::string MarqueeConsole::getText()
{
    return marqueeText;
}

void MarqueeConsole::setText(std::string text)
{
    marqueeText = text;
}

int MarqueeConsole::getdelay() {
    return delay;
}

void MarqueeConsole::setdelay(int newdl) {
    delay = newdl;
}

int MarqueeConsole::getpoll()
{
    return polling;
}

void MarqueeConsole::setpoll(int poll)
{
    polling = poll;
}


void MarqueeConsole::display()
{
    showHeader();
    inputGetter();

}

void MarqueeConsole::setCPos(int x, int y, const std::string& text) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    std::cout << text << std::flush;
}

void MarqueeConsole::process()
{
    running = true;
    bounceThread = std::thread(&MarqueeConsole::bouncer, this);
    inputThread = std::thread(&MarqueeConsole::handleInput, this);
    if (bounceThread.joinable()) bounceThread.join();
    if (inputThread.joinable()) inputThread.join();
}


void MarqueeConsole::bouncer()
{

    int posX = 0, posY = 3;
    int deltaX = 1, deltaY = 1;

    while (running.load()) {
        system("cls");
        display();


        posX += deltaX;
        posY += deltaY;


        if (posX <= 0 || posX + marqueeText.length() >= screenWidth) {
            deltaX *= -1;
        }


        if (posY <= 3 || posY >= 19) {
            deltaY *= -1;
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), currentColor);

        setCPos(posX, posY, marqueeText);

        setCPos(0, 0, "");

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}


void MarqueeConsole::handleInput() {
    while (running.load()) {
        if (_kbhit()) {
            char ch = _getch();

            if (ch == '\r') {

                if (input == "exit") {
                    running = false;
                    system("cls");
                    manager->setActiveConsole("Main");
                }
                else if (input.rfind("color ", 0) == 0) {
                    std::string colorName = input.substr(6);
                    if (colorName == "blue") {
                        currentColor = 1;
                    }
                    else if (colorName == "green") {
                        currentColor = 2;
                    }
                    else if (colorName == "aquamarine") {
                        currentColor = 3;
                    }
                    else if (colorName == "red") {
                        currentColor = 4;
                    }
                    else if (colorName == "purple") {
                        currentColor = 5;
                    }
                    else if (colorName == "yellow") {
                        currentColor = 6;
                    }
                    else if (colorName == "white") {
                        currentColor = 7;
                    }
                    else if (colorName == "gray") {
                        currentColor = 8;
                    }
                    else if (colorName == "light-blue") {
                        currentColor = 9;
                    }
                    commands.push_back("Color now set to " + colorName);
                }
                else if (input.rfind("text ", 0) == 0) {
                    std::string newText = input.substr(5);
                    setText(newText);
                    commands.push_back("Text now set to " + newText);
                }
                else if (input.rfind("delay ", 0) == 0) {
                    std::string newdl_str = input.substr(6);
                    try {
                        int newdl = std::stoi(newdl_str);
                        if (newdl > 9999) {
                            commands.push_back("Delay cannot be set above 10000 because it will be really slow");
                        }
                        else if (newdl < 0) {
                            commands.push_back("Delay cannot be a negative number");
                        }
                        else {
                            setdelay(newdl);
                            commands.push_back("Delay between prints now set to " + std::to_string(newdl));
                        }

                    }
                    catch (const std::invalid_argument& e) {
                        commands.push_back("Invalid input: not a number.");
                    }

                }
                else if (input.rfind("polling ", 0) == 0) {
                    std::string poller_str = input.substr(7);
                    try {
                        int poller = std::stoi(poller_str);
                        if (poller > 9999) {
                            commands.push_back("Polling cannot be set above 10000 because it will be really slow");
                        }
                        else if (poller < 0) {
                            commands.push_back("Polling cannot be a negative number");
                        }
                        else {
                            setpoll(poller);
                            commands.push_back("Keyboard polling delay now set to " + std::to_string(poller));
                        }

                    }
                    catch (const std::invalid_argument& e) {
                        commands.push_back("Invalid input: not a number.");
                    }

                }
                else {
                    commands.push_back(input);
                }

                input.clear();
            }
            else if (ch == '\b') {
                if (!input.empty()) {
                    input.pop_back();
                }
            }
            else {
                input += ch;
            }
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        setCPos(0, 21, std::string(80, ' '));
        setCPos(0, 21, "[User]> " + input);

        setCPos(8 + input.length(), 21, "");

        std::this_thread::sleep_for(std::chrono::milliseconds(polling));
    }

}

void MarqueeConsole::inputGetter() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    setCPos(0, 20, "Enter commands here. Type 'exit' to go back to main menu! Current polling delay: " + std::to_string(getpoll()));
    setCPos(0, 21, "[User]> " + input);
    displayCommands(commands);
}

void MarqueeConsole::displayCommands(const std::vector<std::string>& commands)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    int displayIndex = 0;
    for (int i = commands.size() - 1; i >= 0; i--) {
        setCPos(0, 22 + displayIndex, "Latest Command: " + commands[i]);
        displayIndex++;
    }
}


void MarqueeConsole::showHeader() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    setCPos(0, 0, "*=======================================*");
    setCPos(0, 1, "|      Displaying a Marquee console!    |");
    setCPos(0, 2, "*=======================================*");
    setCPos(95, 0, "Current frame delay: " + std::to_string(getdelay()));
}


void MarqueeConsole::consoleReset() {
    running.store(false);

    if (bounceThread.joinable()) bounceThread.join();
    if (inputThread.joinable()) inputThread.join();

    input.clear();
    commands.clear();
}