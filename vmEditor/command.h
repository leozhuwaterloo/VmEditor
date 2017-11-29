#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <memory>
#include <initializer_list>
#include <vector>

class Window;
class Event;
class Cursor;

class Command{
private:
    std::vector<int> keys;
public:
    Command(std::initializer_list<int> keys);
    virtual ~Command() = default;
    const std::vector<int>& getKeys() const;
    virtual std::unique_ptr<Event> execute(Window *w) const;
    virtual void run(Window *w) const = 0;
};

class UndoableCommand: public Command{
public:
    UndoableCommand(std::initializer_list<int> keys);
    virtual std::unique_ptr<Event> execute(Window *w) const override;
    virtual void run(Window *w) const override;
    virtual std::unique_ptr<Event> runEvent(Window *w) const = 0;
    virtual void reverseExecute(Window *w, Event *e) const = 0;
};

class Commandi: public UndoableCommand{
public:
    Commandi();
    virtual std::unique_ptr<Event> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

class Commanda: public Command{
public:
    Commanda();
    virtual void run(Window *w) const override;
};

class CommandUp: public Command{
public:
    CommandUp();
    virtual void run(Window *w) const override;
};

class CommandDown: public Command{
public:
    CommandDown();
    virtual void run(Window *w) const override;
};

class CommandRight: public Command{
public:
    CommandRight();
    virtual void run(Window *w) const override;
};

class CommandLeft: public Command{
public:
    CommandLeft();
    virtual void run(Window *w) const override;
};

class CommandResize: public Command{
public:
    CommandResize();
    virtual void run(Window *w) const override;
};

class Commandw: public Command {
public:
    Commandw();
    virtual void run(Window *w) const override;
};

class Commandb: public Command {
public:
    Commandb();
    virtual void run(Window *w) const override;
};

class Command0: public Command {
public:
    Command0();
    virtual void run(Window *w) const override;
};

class CommandDollar: public Command {
public:
    CommandDollar();
    virtual void run(Window *w) const override;
};

class CommandCaret: public Command {
public:
    CommandCaret();
    virtual void run(Window *w) const override;
};

class Commandu: public Command {
public:
    Commandu();
    virtual void run(Window *w) const override;
};

#endif
