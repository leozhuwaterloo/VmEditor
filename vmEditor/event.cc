#include "event.h"
#include "command.h"
#include "store.h"

Event::Event(const UndoableCommand *command):command{command}{}
void Event::reExecute(Window *w){ command->execute(w); }
void Event::reverseExecute(Window *w){ command->reverseExecute(w, this); }


InsertEvent::InsertEvent(const UndoableCommand *command, std::list<std::string>::iterator itLst,
    std::string::iterator itStr, std::vector<int> inserted):Event{command}, itLst{itLst}, itStr{itStr}, inserted{inserted}{}
