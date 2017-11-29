#ifndef EVENT_H
#define EVENT_H
#include <vector>
#include <list>
#include <string>

class UndoableCommand;
class Window;
class Store;

class Event{
private:
    const UndoableCommand *command;
public:
    Event(const UndoableCommand *command);
    virtual ~Event() = default;
    void reExecute(Window *w);
    void reverseExecute(Window *w);
};


class InsertEvent: public Event{
public:
    std::list<std::string>::iterator itLst;
    std::string::iterator itStr;
    std::vector<int> inserted;
    InsertEvent(const UndoableCommand *command, std::list<std::string>::iterator itLst,
        std::string::iterator itStr, std::vector<int> inserted);
};

#endif
