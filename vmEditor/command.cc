#include "command.h"
#include "window.h"

Command::Command(std::initializer_list<int> keys):keys{keys}{}
UndoableCommand::UndoableCommand(std::initializer_list<int> keys):Command{keys}{}
CommandInsert::CommandInsert():Command{105}{}
CommandUp::CommandUp():Command{65}{}
CommandDown::CommandDown():Command{66}{}
CommandRight::CommandRight():Command{67}{}
CommandLeft::CommandLeft():Command{68}{}

const std::vector<int>& Command::getKeys() const{
    return keys;
}

void CommandInsert::execute(Window *w) const{
    w->showStatus("-- INSERT -- ");
}

void CommandUp::execute(Window *w) const{ w->getCursor()->moveY(-1); }
void CommandDown::execute(Window *w) const{ w->getCursor()->moveY(1); }
void CommandRight::execute(Window *w) const{ w->getCursor()->moveX(1); }
void CommandLeft::execute(Window *w) const{ w->getCursor()->moveX(-1); }
