#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <memory>
#include <initializer_list>
#include <vector>

class Window;

class Command{
private:
    std::vector<int> keys;
public:
    Command(std::initializer_list<int> keys);
    virtual ~Command() = default;
    const std::vector<int>& getKeys() const;
    virtual void execute(Window *w) const = 0;
};

class UndoableCommand: public Command{
public:
    UndoableCommand(std::initializer_list<int> keys);
    virtual void reverse_execute(Window *w) const = 0;
};

class CommandInsert: public Command{
public:
    CommandInsert();
    virtual void execute(Window *w) const override;
};

class CommandUp: public Command{
public:
    CommandUp();
    virtual void execute(Window *w) const override;
};

class CommandDown: public Command{
public:
    CommandDown();
    virtual void execute(Window *w) const override;
};

class CommandRight: public Command{
public:
    CommandRight();
    virtual void execute(Window *w) const override;
};

class CommandLeft: public Command{
public:
    CommandLeft();
    virtual void execute(Window *w) const override;
};

class CommandResize: public Command{
public:
    CommandResize();
    virtual void execute(Window *w) const override;
};

class CommandW: public Command {
public:
    CommandW();
    virtual void execute(Window *w) const override;
};

class CommandB: public Command {
public:
    CommandB();
    virtual void execute(Window *w) const override;
};

#endif
