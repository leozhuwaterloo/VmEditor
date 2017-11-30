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
    virtual std::vector<std::unique_ptr<Event>> execute(Window *w) const;
    virtual void run(Window *w) const = 0;
};

class UndoableCommand: public Command{
public:
    UndoableCommand(std::initializer_list<int> keys);
    virtual std::vector<std::unique_ptr<Event>> execute(Window *w) const override;
    virtual void run(Window *w) const override;
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const = 0;
    virtual void reverseExecute(Window *w, Event *e) const = 0;
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

class Commandh: public Command{
public:
    Commandh();
    virtual void run(Window *w) const override;
};

class Commandl: public Command{
public:
    Commandl();
    virtual void run(Window *w) const override;
};

class Commandk: public Command{
public:
    Commandk();
    virtual void run(Window *w) const override;
};

class Commandj: public Command{
public:
    Commandj();
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

class Commandf: public Command {
public:
    Commandf();
    virtual void run(Window *w) const override;
};

class CommandF: public Command {
public:
    CommandF();
    virtual void run(Window *w) const override;
};

class Commandi: public UndoableCommand{
public:
    Commandi();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

class Commanda: public UndoableCommand{
public:
    Commanda();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

class Commandx: public UndoableCommand {
public:
    Commandx();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

class CommandX: public UndoableCommand {
public:
    CommandX();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};
class Commandr: public UndoableCommand {
public:
    Commandr();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

class CommandR: public UndoableCommand {
public:
    CommandR();
    virtual std::vector<std::unique_ptr<Event>> runEvent(Window *w) const override;
    virtual void reverseExecute(Window *w, Event *e) const override;
};

#endif
