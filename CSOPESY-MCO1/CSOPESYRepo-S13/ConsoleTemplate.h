#pragma once

class ConsoleTemplate {
public:
    ConsoleTemplate() = default;
    virtual ~ConsoleTemplate() = default;

    virtual void onEnabled() = 0;
    virtual void display() = 0;
    virtual void process() = 0;
};