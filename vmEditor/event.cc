#include "event.h"
#include "command.h"
#include "store.h"
#include "window.h"
#include "keylistener.h"

Event::Event(const UndoableCommand *command):command{command}{}
std::vector<std::unique_ptr<Event>> Event::reExecute(Window *w){ std::vector<std::unique_ptr<Event>> events; command->execute(w);  return events;}
void Event::restore(Window *w){ command->reverseExecute(w, this); }



StoreChangeEvent::StoreChangeEvent(const UndoableCommand *command, std::unique_ptr<Store> preStore,
    int cursorY, int cursorX, const std::vector<int> &triggered):Event{command}, preStore{std::move(preStore)}, cursorY{cursorY}, cursorX{cursorX}{
    this->triggered.assign(triggered.begin(), triggered.end());
}
std::vector<std::unique_ptr<Event>> StoreChangeEvent::reExecute(Window *w){ w->getKeyListener()->pushStream(triggered); return command->execute(w); }
std::unique_ptr<Store> &StoreChangeEvent::getPreStore(){ return preStore; }
int StoreChangeEvent::getCursorY() const { return cursorY; }
int StoreChangeEvent::getCursorX() const { return cursorX; }
