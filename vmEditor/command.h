#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <memory>

class Window;

class Command{
private:
    std::string keyString;
public:
    Command(std::string keyString);
    virtual ~Command() = default;
    std::string getKeyString() const;
    virtual void execute(Window *window) const = 0;
};

class UndoableCommand: public Command{
public:
    UndoableCommand(std::string keyString);
    virtual void reverse_execute(Window *window) const = 0;
};

class CommandInsert: public Command{
public:
    CommandInsert();
    virtual void execute(Window *window) const override;
};

#endif
