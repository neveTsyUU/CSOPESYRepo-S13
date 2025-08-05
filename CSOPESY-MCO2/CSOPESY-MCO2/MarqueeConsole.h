#pragma once
#include "ConsoleManager.h"
#include "ConsoleTemplate.h"
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <chrono>

class MarqueeConsole : public ConsoleTemplate {
public:
    MarqueeConsole(const std::string& name, std::shared_ptr<ConsoleManager> mgr);
    ~MarqueeConsole();
    void display() override;
    void process() override;
    void onEnabled() override;
    std::string getText();
    void setText(std::string text);
    int getdelay();
    void setdelay(int delay);
    int getpoll();
    void setpoll(int polling);


private:
    std::shared_ptr<ConsoleManager> manager;
    std::string input;
    std::vector<std::string> commands;

    void bouncer();
    void handleInput();        // Input handling loop
    void showHeader();
    void setCPos(int x, int y, const std::string& text);
    void consoleReset();
    void displayCommands(const std::vector<std::string>& commands);
    void inputGetter();
    int screenWidth = 120;
    int screenHeight = 30;
    int delay = 60;
    int polling = 10;
    int currentColor = 7;
    std::string marqueeText = "Hello world in marquee!";
    std::atomic<bool> running{ true };
    std::thread bounceThread;
    std::thread inputThread;
};

