#ifndef KEYLISTENER_H
#define KEYLISTENER_H
#include <string>
#include <map>
#include <memory>
#include "command.h"

class Window;

class KeyListener{
private:
    std::map<std::string, std::unique_ptr<Command>> commands;
    std::string currCommand;
public:
    KeyListener() = default;
    ~KeyListener() = default;
    void init(Window *window);
    void addCommand(std::unique_ptr<Command> command);
};

#endif
