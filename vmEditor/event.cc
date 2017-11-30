#include "event.h"
#include "command.h"
#include "store.h"

Event::Event(const UndoableCommand *command):command{command}{}
void Event::reExecute(Window *w){ command->execute(w); }
void Event::restore(Window *w){ command->reverseExecute(w, this); }


StoreChangeEvent::StoreChangeEvent(const UndoableCommand *command, std::unique_ptr<Store> preStore,
    int cursorY, int cursorX):Event{command}, preStore{std::move(preStore)}, cursorY{cursorY}, cursorX{cursorX}{}
std::unique_ptr<Store> &StoreChangeEvent::getPreStore(){ return preStore; }
int StoreChangeEvent::getCursorY() const { return cursorY; }
int StoreChangeEvent::getCursorX() const { return cursorX; }
