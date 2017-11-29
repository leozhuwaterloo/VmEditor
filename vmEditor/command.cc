#include "command.h"
#include "window.h"
#include "keylistener.h"
#include "event.h"
#include "store.h"
#include <locale>
#include <ncurses.h>
#include <stack>

/*
https://www.fprintf.net/vimCheatSheet.html
*/

/*
cc c[any motion] dd d[any motion] f h j k l n o p q r s u x yy y[any motion]
A F I J N O P R S X . ; / ? % @
^b ^d ^f ^g ^u
*/

Command::Command(std::initializer_list<int> keys):keys{keys}{}

const std::vector<int>& Command::getKeys() const{
    return keys;
}

std::unique_ptr<Event> Command::execute(Window *w) const{
    run(w);
    return nullptr;
}

UndoableCommand::UndoableCommand(std::initializer_list<int> keys):Command{keys}{}
void UndoableCommand::run(Window *w) const{}
std::unique_ptr<Event> UndoableCommand::execute(Window *w) const{ return runEvent(w); }


Commandi::Commandi():UndoableCommand{105}{}
Commanda::Commanda():Command{97}{}
CommandUp::CommandUp():Command{259}{}
CommandDown::CommandDown():Command{258}{}
CommandRight::CommandRight():Command{261}{}
CommandLeft::CommandLeft():Command{260}{}
CommandResize::CommandResize():Command{410}{}
Commandw::Commandw():Command{119}{}
Commandb::Commandb():Command{98}{}
Command0::Command0():Command{48}{}
CommandDollar::CommandDollar():Command{36}{}
CommandCaret::CommandCaret():Command{94}{}
Commandu::Commandu():Command{117}{}

std::vector<int> enterInsertMode(Window *w, const int &initShift){
    w->showStatus("-- INSERT -- ");
    w->setState(STATE_INSERT);
    w->getCursor()->moveX(initShift);
    int ch;
    std::vector<int> inserted;
    while (ch = getch()) {
        inserted.push_back(ch);
        if (ch == 27) break;  // escape
        else if (ch == 259) w->getCursor()->moveY(-1);
        else if (ch == 258) w->getCursor()->moveY(1);
        else if (ch == 260) w->getCursor()->moveX(-1);
        else if (ch == 261) w->getCursor()->moveX(1);
        else if (ch == 410) w->resize();
        else if (ch == 8 || ch == 263) {    // backspace
            w->getCursor()->erase();
            w->render();
        } else {
            w->getCursor()->insert(ch);
            w->render();
        }
    }
    w->setState(STATE_NORMAL);
    w->getCursor()->moveX(-1);
    w->showStatus("");
    return inserted;
}


std::unique_ptr<Event> Commandi::runEvent(Window *w) const{
    return std::make_unique<InsertEvent>(this, w->getCursor()->getItLst(), w->getCursor()->getItStr(), enterInsertMode(w, 0));
}
void Commandi::reverseExecute(Window *w, Event *e) const{
    InsertEvent* insertEvent = dynamic_cast<InsertEvent*>(e);
    if(insertEvent){
        w->getCursor()->getItLst() = insertEvent->itLst;
        w->getCursor()->getItStr() = insertEvent->itStr;

        w->setState(STATE_INSERT);
        w->getCursor()->moveX(1);
        for(auto &it: insertEvent->inserted){

            // maybe it should copy the entire store instead

        }
        w->setState(STATE_NORMAL);

        w->getCursor()->adjust();
        w->render();
    }
}
void Commanda::run(Window *w) const{ enterInsertMode(w, 1); }

void CommandUp::run(Window *w) const{ w->getCursor()->moveY(-1); }
void CommandDown::run(Window *w) const{ w->getCursor()->moveY(1); }
void CommandRight::run(Window *w) const{ w->getCursor()->moveX(1); }
void CommandLeft::run(Window *w) const{ w->getCursor()->moveX(-1); }

void CommandResize::run(Window *w) const{ w->resize(); }

void Commandw::run(Window *w) const{ // words forward
    if (w->getCursor()->isAtEnd()) return;
    if (w->getCursor()->isAtEmptyLine() || w->getCursor()->isAtLineEnd()){
        w->getCursor()->moveOne(1);
    }else{
        int isAlnum = std::isalnum(w->getCursor()->currChar());
        do {
            if(std::isspace(w->getCursor()->currChar())) break;
            w->getCursor()->moveOne(1);
        } while (!w->getCursor()->isAtEnd() && std::isalnum(w->getCursor()->currChar()) == isAlnum);
    }
    // move on if currChar is whitespace
    while (!w->getCursor()->isAtEnd() && std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(1);
    }
}

void Commandb::run(Window *w) const{ // words backward
    if (w->getCursor()->isAtBegin()) return;
    w->getCursor()->moveOne(-1);
    if (w->getCursor()->isAtLineBegin()) return;

    while (!w->getCursor()->isAtBegin() && std::isspace(w->getCursor()->currChar())) {
        w->getCursor()->moveOne(-1);
    }

    int isAlnum = std::isalnum(w->getCursor()->currChar());
    do {
        w->getCursor()->moveOne(-1);
        if(std::isspace(w->getCursor()->currChar())) break;
    } while (!w->getCursor()->isAtBegin() && std::isalnum(w->getCursor()->currChar()) == isAlnum);
    w->getCursor()->moveOne(1);
}

void Command0::run(Window *w) const{ w->getCursor()->moveLineBegin(); } // To the first character of the line
void CommandDollar::run(Window *w) const{ w->getCursor()->moveLineEnd(); } // To the end of the line
void CommandCaret::run(Window *w) const{ w->getCursor()->moveLineBeginNonWs(); }  // To the first non-blank character of the line


void Commandu::run(Window *w) const{  // undo
    std::stack<std::unique_ptr<Event>> &eventHistory = w->getKeyListener()->getEventHistory();
    if(eventHistory.empty()){
        w->showStatus("Already at oldest change");
    } else {
        eventHistory.top()->reverseExecute(w);
        eventHistory.pop();
    }
}
