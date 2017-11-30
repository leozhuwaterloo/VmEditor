#ifndef EVENT_H
#define EVENT_H
#include <vector>
#include <list>
#include <string>
#include <memory>

class UndoableCommand;
class Window;
class Store;

class Event{
private:
    const UndoableCommand *command;
public:
    Event(const UndoableCommand *command);
    virtual ~Event() = default;
    virtual void reExecute(Window *w);
    virtual void restore(Window *w);
};


class StoreChangeEvent: public Event{
private:
    std::unique_ptr<Store> preStore;
    int cursorY, cursorX;
public:
    StoreChangeEvent(const UndoableCommand *command, std::unique_ptr<Store> preStore, int cursorY, int cursorX);
    std::unique_ptr<Store> &getPreStore();
    int getCursorY() const;
    int getCursorX() const;
};

#endif
