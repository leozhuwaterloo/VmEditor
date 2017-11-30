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
cc c[any motion] dd d[any motion] f h j k l n o p q r s yy y[any motion]
A F I J N O P R S . ; / ? % @
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
Commandf::Commandf():Command{102}{}
CommandF::CommandF():Command{70}{}

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
        std::unique_ptr<Event> &topEvent = eventHistory.top();
        topEvent->restore(w);
        eventHistory.pop();
        topEvent.reset();
    }
}

void find(Window *w, const int &direction, const char &target){
    std::string::iterator initIt = w->getCursor()->getItStr();
    bool found = false;
    do{
        w->getCursor()->moveX(direction);
        if (w->getCursor()->currChar() == target) { found = true; break;}
    } while(!w->getCursor()->isAtLineEnd() && !w->getCursor()->isAtLineBegin());

    if(!found){
        while(w->getCursor()->getItStr() != initIt){
            w->getCursor()->moveX(-direction);
        }
    }
}

void Commandf::run(Window *w) const{ find(w, 1, getch()); } // To [count]'th occurrence of {char} to the right. The cursor is placed on {char}
void CommandF::run(Window *w) const{ find(w, -1, getch()); } // To the [count]'th occurrence of {char} to the left. The cursor is placed on {char} (inclusive).


Commandi::Commandi():UndoableCommand{105}{}
Commanda::Commanda():UndoableCommand{97}{}
Commandx::Commandx():UndoableCommand{120}{}
CommandX::CommandX():UndoableCommand{88}{}


std::unique_ptr<StoreChangeEvent> enterInsertMode(const UndoableCommand *command, Window *w, const int &initShift){
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    w->showStatus("-- INSERT -- ");
    w->setState(STATE_INSERT);
    w->getCursor()->moveX(initShift);
    int ch;
    bool inserted = false;
    while (ch = getch()) {
        if (ch == 27) break;  // escape
        else if (ch == 259) w->getCursor()->moveY(-1);
        else if (ch == 258) w->getCursor()->moveY(1);
        else if (ch == 260) w->getCursor()->moveX(-1);
        else if (ch == 261) w->getCursor()->moveX(1);
        else if (ch == 410) w->resize();
        else if (ch == 8 || ch == 263) {    // backspace
            w->getCursor()->erase();
            inserted = true;
            w->render();
        } else {
            w->getCursor()->insert(ch);
            inserted = true;
            w->render();
        }
    }
    w->setState(STATE_NORMAL);
    w->getCursor()->moveX(-1);
    w->showStatus("");

    if(inserted){
        return std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX);
    } else{
        preStore.reset();
        return nullptr;
    }
}

void restoreStore(Window *w, StoreChangeEvent* storeChangeEvent){
    if(storeChangeEvent){
        w->getUniqueStore().swap(storeChangeEvent->getPreStore());
        std::list<std::string>::iterator &itLst = w->getCursor()->getItLst();
        std::string::iterator &itStr = w->getCursor()->getItStr();
        itLst = w->getStore()->getStrs().begin();
        for(int i = 0; i < storeChangeEvent->getCursorY(); ++i) ++itLst;
        itStr = w->getCursor()->getItLst()->begin();
        itStr += storeChangeEvent->getCursorX();
        w->getCursor()->adjust();
        w->render();
    }
}

std::unique_ptr<Event> Commandi::runEvent(Window *w) const{ return enterInsertMode(this, w, 0); }
void Commandi::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::unique_ptr<Event> Commanda::runEvent(Window *w) const{ return enterInsertMode(this, w, 1); }
void Commanda::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::unique_ptr<StoreChangeEvent> deleteOne(const UndoableCommand *command, Window *w, const int &shift){
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    w->setState(STATE_INSERT);
    w->getCursor()->moveX(shift);
    bool erased = false;
    if (!w->getCursor()->isAtLineBegin()){
        w->getCursor()->erase();
        erased = true;
    }
    w->setState(STATE_NORMAL);
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    if (erased){
        if(w->getCursor()->isAtLineEnd(1)) w->getCursor()->moveX(-1);
        w->render();
        return std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX);
    } else{
        preStore.reset();
        return nullptr;
    }
}

std::unique_ptr<Event> Commandx::runEvent(Window *w) const{ return deleteOne(this, w, 1); } // Delete characters under and after the cursor
void Commandx::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::unique_ptr<Event> CommandX::runEvent(Window *w) const{ return deleteOne(this, w, 0); } //Delete characters before the cursor
void CommandX::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
