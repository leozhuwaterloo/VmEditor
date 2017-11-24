#include "command.h"
#include "window.h"
#include <locale>
#include <ncurses.h>

Command::Command(std::initializer_list<int> keys):keys{keys}{}
UndoableCommand::UndoableCommand(std::initializer_list<int> keys):Command{keys}{}
CommandInsert::CommandInsert():Command{105}{}
CommandUp::CommandUp():Command{259}{}
CommandDown::CommandDown():Command{258}{}
CommandRight::CommandRight():Command{261}{}
CommandLeft::CommandLeft():Command{260}{}
CommandResize::CommandResize():Command{410}{}
CommandW::CommandW():Command{119}{}
CommandB::CommandB():Command{98}{}
Command0::Command0():Command{48}{}
CommandDollar::CommandDollar():Command{36}{}
CommandCaret::CommandCaret():Command{94}{}

const std::vector<int>& Command::getKeys() const{
    return keys;
}

void CommandInsert::execute(Window *w) const{
    w->showStatus("-- INSERT -- ");
    int ch;
    while (ch = getch()) {
        if (ch == 27) break;  // escape
        else if (ch == 259) w->getCursor()->moveY(-1);
        else if (ch == 258) w->getCursor()->moveY(1);
        else if (ch == 260) w->getCursor()->moveX(-1);
        else if (ch == 261)
            w->getCursor()->isAtLineEnd() ? w->getCursor()->moveOnePastEnd() : w->getCursor()->moveX(1);
        else if (ch == 8 || ch == 127) {    // backspace
            w->showStatus("backspace");
        } else {
            w->getCursor()->insert(ch);
            w->render();
            w->showStatus("inserting...");
        }
    }
    w->getCursor()->moveX(-1);
    w->showStatus("");
}

void CommandUp::execute(Window *w) const{ w->getCursor()->moveY(-1); }
void CommandDown::execute(Window *w) const{ w->getCursor()->moveY(1); }
void CommandRight::execute(Window *w) const{ w->getCursor()->moveX(1); }
void CommandLeft::execute(Window *w) const{ w->getCursor()->moveX(-1); }

void CommandResize::execute(Window *w) const{ w->resize(); }

void CommandW::execute(Window *w) const{
    if (w->getCursor()->isEmptyLine()) {
        w->getCursor()->moveOne(1);
        return;
    }
    if (std::isalnum(w->getCursor()->currChar())) {
        do {
            w->getCursor()->moveOne(1);
            if(w->getCursor()->isAtLineEnd()) {
                w->getCursor()->moveOne(1);
                break;
            }
        } while (!w->getCursor()->isAtEnd() && std::isalnum(w->getCursor()->currChar()));
    } else {
        do {
            w->getCursor()->moveOne(1);
            if (w->getCursor()->isEmptyLine()) return;
            if(w->getCursor()->isAtLineEnd()) {
                w->getCursor()->moveOne(1);
                break;
            }
        } while (!w->getCursor()->isAtEnd() && !std::isalnum(w->getCursor()->currChar()));
    }

    while (!w->getCursor()->isAtEnd() && std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(1);
    }
}
void CommandB::execute(Window *w) const{
    if (w->getCursor()->isEmptyLine()) {
        w->getCursor()->moveOne(-1);
        return;
    }
    if (std::isalnum(w->getCursor()->currChar())) {
        do {
            w->getCursor()->moveOne(-1);
            if(w->getCursor()->isAtLineBegin()) {
                w->getCursor()->moveOne(-1);
                break;
            }
        } while (!w->getCursor()->isAtBegin() && std::isalnum(w->getCursor()->currChar()));
    } else {
        do {
            w->getCursor()->moveOne(-1);
            if (w->getCursor()->isEmptyLine()) return;
            if(w->getCursor()->isAtLineBegin()) {
                w->getCursor()->moveOne(-1);
                break;
            }
        } while (!w->getCursor()->isAtBegin() && !std::isalnum(w->getCursor()->currChar()));
    }

    while (!w->getCursor()->isAtBegin() && std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(-1);
    }
}
void Command0::execute(Window *w) const{ w->getCursor()->moveLineBegin(); }
void CommandDollar::execute(Window *w) const{ w->getCursor()->moveLineEnd(); }
void CommandCaret::execute(Window *w) const{ w->getCursor()->moveFirstNonWs(); }
