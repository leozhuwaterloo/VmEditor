#ifndef KEYLISTENER_H
#define KEYLISTENER_H
#include <map>
#include <vector>
#include <memory>

class Command;
class Window;

class KeyListener{
private:
    std::map<std::vector<int>, std::unique_ptr<Command>> commands;
    std::vector<int> currKeys;
public:
    KeyListener() = default;
    ~KeyListener() = default;
    void init(Window *window);
    void addCommand(std::unique_ptr<Command> command);
};

#endif
