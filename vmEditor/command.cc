#include "command.h"
#include "window.h"
#include <locale>

Command::Command(std::initializer_list<int> keys):keys{keys}{}
UndoableCommand::UndoableCommand(std::initializer_list<int> keys):Command{keys}{}
CommandInsert::CommandInsert():Command{105}{}
CommandUp::CommandUp():Command{65}{}
CommandDown::CommandDown():Command{66}{}
CommandRight::CommandRight():Command{67}{}
CommandLeft::CommandLeft():Command{68}{}
CommandResize::CommandResize():Command{410}{}
CommandW::CommandW():Command{119}{}
CommandB::CommandB():Command{98}{}

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

void CommandResize::execute(Window *w) const{ w->resize(); }

void CommandW::execute(Window *w) const{
    if (std::isalnum(w->getCursor()->currChar())) {
        do {
            w->getCursor()->moveOne(1);
        } while (std::isalnum(w->getCursor()->currChar()));
    } else {
        do {
            w->getCursor()->moveOne(1);
        } while (!std::isalnum(w->getCursor()->currChar()));
    }

    while (std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(1);
    }
}
void CommandB::execute(Window *w) const{
    if (std::isalnum(w->getCursor()->currChar())) {
        do {
            w->getCursor()->moveOne(-1);
        } while (std::isalnum(w->getCursor()->currChar()));
    } else {
        do {
            w->getCursor()->moveOne(-1);
        } while (!std::isalnum(w->getCursor()->currChar()));
    }

    while (std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(-1);
    }
}
