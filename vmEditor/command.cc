#include "command.h"
#include "window.h"
#include "keylistener.h"
#include "event.h"
#include "store.h"
#include "utils.h"
#include <locale>
#include <ncurses.h>
#include <stack>
#include <regex>
#include <sstream>
#include "parser.h"
#include "saver.h"

/*
%
*/

Command::Command(std::initializer_list<int> keys):keys{keys}{}

const std::vector<int>& Command::getKeys() const{
    return keys;
}

std::vector<std::unique_ptr<Event>> Command::execute(Window *w) const{
    std::vector<std::unique_ptr<Event>> empty;
    run(w);
    return empty;
}

UndoableCommand::UndoableCommand(std::initializer_list<int> keys):Command{keys}{}
void UndoableCommand::run(Window *w) const{}
std::vector<std::unique_ptr<Event>> UndoableCommand::execute(Window *w) const{ return runEvent(w); }


CommandUp::CommandUp():Command{259}{}
CommandDown::CommandDown():Command{258}{}
CommandRight::CommandRight():Command{261}{}
CommandLeft::CommandLeft():Command{260}{}
Commandh::Commandh():Command{104}{}
Commandl::Commandl():Command{108}{}
Commandk::Commandk():Command{107}{}
Commandj::Commandj():Command{106}{}
CommandResize::CommandResize():Command{410}{}
Commandw::Commandw():Command{119}{}
Commandb::Commandb():Command{98}{}
Command0::Command0():Command{48}{}
CommandDollar::CommandDollar():Command{36}{}
CommandCaret::CommandCaret():Command{94}{}
Commandu::Commandu():Command{117}{}
Commandf::Commandf():Command{102}{}
CommandF::CommandF():Command{70}{}
CommandSlash::CommandSlash():Command{47}{}
CommandQuestion::CommandQuestion():Command{63}{}
Commandn::Commandn():Command{110}{}
CommandN::CommandN():Command{78}{}
CommandCtrlf::CommandCtrlf():Command{6}{}
CommandCtrlg::CommandCtrlg():Command{7}{}
Commandq::Commandq():Command{113}{}
CommandSemi::CommandSemi():Command{59}{}
CommandCtrld::CommandCtrld():Command{4}{}
CommandCtrlu::CommandCtrlu():Command{21}{}
CommandCtrlb::CommandCtrlb():Command{2}{}

void CommandUp::run(Window *w) const{ w->getCursor()->moveY(-1); }
void CommandDown::run(Window *w) const{ w->getCursor()->moveY(1); }
void CommandRight::run(Window *w) const{ w->getCursor()->moveX(1); }
void CommandLeft::run(Window *w) const{ w->getCursor()->moveX(-1); }

void Commandh::run(Window *w) const{ w->getCursor()->moveX(-1); }
void Commandl::run(Window *w) const{ w->getCursor()->moveX(1); }
void Commandk::run(Window *w) const{ w->getCursor()->moveY(-1); }
void Commandj::run(Window *w) const{ w->getCursor()->moveY(1); }

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

void Command0::run(Window *w) const{ w->getCursor()->moveLineBegin(); }
void CommandDollar::run(Window *w) const{ w->getCursor()->moveLineEnd(); }
void CommandCaret::run(Window *w) const{ w->getCursor()->moveLineBeginNonWs(); }


void Commandu::run(Window *w) const{
    std::stack<std::unique_ptr<Event>> &eventHistory = w->getKeyListener()->getEventHistory();
    if(eventHistory.empty()){
        w->showStatus("Already at oldest change");
    } else {
        std::unique_ptr<Event> &topEvent = eventHistory.top();
        topEvent->restore(w);
        eventHistory.pop();
        topEvent.reset();
        if(eventHistory.empty()) w->getSaver()->setModified(false);
    }
}

void find(Window *w, const int &direction, const char &target){
    w->getKeyListener()->setLatestFind(target, direction);
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

void Commandf::run(Window *w) const{ find(w, 1, w->getKeyListener()->get()); }
void CommandF::run(Window *w) const{ find(w, -1, w->getKeyListener()->get()); }

void startSearch(Window *w, const std::string &target, const int &direction){
    if(target.empty()) return;
    w->getCursor()->setState(STATE_EDIT);
    std::list<std::string>::iterator initItLst = w->getCursor()->getItLst();
    size_t found;
    if(direction > 0) found = initItLst->find(target, w->getCursor()->getItStr() - initItLst->begin() + 1);
    else found = initItLst->substr(0, w->getCursor()->getItStr() - initItLst->begin()).find(target);

    while(found == std::string::npos){
        if(direction >0 && std::next(w->getCursor()->getItLst(), 1) == w->getStore()->getStrs().end()) break;
        else if(direction <= 0 && w->getCursor()->getItLst() == w->getStore()->getStrs().begin()) break;

        if(direction > 0) ++(w->getCursor()->getItLst());
        else --(w->getCursor()->getItLst());

        if(direction > 0) found = w->getCursor()->getItLst()->find(target);
        else found = w->getCursor()->getItLst()->rfind(target);
    }

    if(found != std::string::npos){
        w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin() + found;
        w->getCursor()->adjust();
        w->refreshCursor();
    }else{
        w->getCursor()->getItLst() =  initItLst;
    }
}

void longSearch(const char &commandChar, Window *w, const int &direction){
    set_escdelay(1000);
    w->getCursor()->setState(STATE_STATUS);
    std::string commandSymbol = std::string(1, commandChar);
    std::string target;
    w->showStatus(commandSymbol);
    w->getCursor()->moveTo(w->getMaxY()-1, 1);
    int ch;
    while ((ch = w->getKeyListener()->get())){
        if(ch == 10){
            if(target.empty() && w->getKeyListener()->getSearched()){
                target = w->getKeyListener()->getLatestSearch().first;
            }
            startSearch(w, target, direction);
            w->getKeyListener()->setLatestSearch(target, direction);
            break;
        }
        else if(ch == 27) break;
        else if (ch == 410) w->resize();
        else{
            if(ch == 8 || ch == 263){
                if(target.empty()) break;
                target = target.substr(0, target.length()-1);
            } else {
                target += std::string(1, ch);
            }
            w->showStatus(commandSymbol + target);
            w->getCursor()->moveTo(w->getMaxY()-1, target.length() + 1);
        }
    }
    w->getCursor()->setState(STATE_EDIT);
    w->showStatus("");
    set_escdelay(0);
}


void CommandSlash::run(Window *w) const{ longSearch(getKeys().at(0), w, 1); }
void CommandQuestion::run(Window *w) const{ longSearch(getKeys().at(0), w, -1); }

void Commandn::run(Window *w) const{
    if(!w->getKeyListener()->getSearched()) return;
    std::pair<std::string, int> latestSearch = w->getKeyListener()->getLatestSearch();
    startSearch(w, latestSearch.first, latestSearch.second);
}
void CommandN::run(Window *w) const{
    if(!w->getKeyListener()->getSearched()) return;
    std::pair<std::string, int> latestSearch = w->getKeyListener()->getLatestSearch();
    startSearch(w, latestSearch.first, (0 - latestSearch.second));
}

void CommandCtrlf::run(Window *w) const{
    while(std::next(w->getStore()->getItCurrY(), 1) != w->getStore()->getStrs().end()){
        w->getStore()->moveY(1);
    }
    w->getCursor()->getItLst() = std::next(w->getStore()->getStrs().end(), -1);
    w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin();
    w->getCursor()->adjust();
    w->render();
}

void CommandCtrlg::run(Window *w) const{
    bool modified = w->getSaver()->getModified();
    int total_line = w->getStore()->getStrs().size();
    int curr_line = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst()) + 1;
    w->showStatus("\""+ w->getParser()->getFileName() + "\" " + (modified ? "[Modified] " : "") + std::to_string(total_line) +
        " lines --" + std::to_string(curr_line * 100 / total_line) + "%--");
}

void Commandq::run(Window *w) const{
    if(w->getKeyListener()->isRecording()){
        w->getKeyListener()->stopRecording();
    }else{
        int ch = w->getKeyListener()->get();
        if(std::isalnum(ch)) w->getKeyListener()->startRecording(ch);
    }
    w->showStatus("");
}

void CommandSemi::run(Window *w) const{
    if(!w->getKeyListener()->getFinded()) return;
    std::pair<char, int> latestFind = w->getKeyListener()->getLatestFind();
    find(w, latestFind.second, latestFind.first);
}

void CommandCtrld::run(Window *w) const{
    for(int i = 0; i < w->getStore()->getDisplayedLine() / 2; ++i){
        w->getCursor()->moveY(1);
    }
}

void CommandCtrlu::run(Window *w) const{
    for(int i = 0; i < w->getStore()->getDisplayedLine() / 2; ++i){
        w->getCursor()->moveY(-1);
    }
}

void CommandCtrlb::run(Window *w) const{
    for(int i = 0; i < w->getStore()->getDisplayedLine() - 1; ++i){
        w->getCursor()->moveY(-1);
    }
}

Commandi::Commandi():UndoableCommand{105}{}
CommandI::CommandI():UndoableCommand{73}{}
Commanda::Commanda():UndoableCommand{97}{}
CommandA::CommandA():UndoableCommand{65}{}
Commandx::Commandx():UndoableCommand{120}{}
CommandX::CommandX():UndoableCommand{88}{}
Commandr::Commandr():UndoableCommand{114}{}
CommandR::CommandR():UndoableCommand{82}{}
CommandJ::CommandJ():UndoableCommand{74}{}
Commando::Commando():UndoableCommand{111}{}
CommandO::CommandO():UndoableCommand{79}{}
CommandColon::CommandColon():UndoableCommand{58}{}
CommandAt::CommandAt():UndoableCommand{64}{}
CommandDot::CommandDot():UndoableCommand{46}{}
Commandc::Commandc():UndoableCommand{99}{}
Commandd::Commandd():UndoableCommand{100}{}
Commandy::Commandy():UndoableCommand{121}{}
Commandp::Commandp():UndoableCommand{112}{}
CommandP::CommandP():UndoableCommand{80}{}
Commands::Commands():UndoableCommand{115}{}
CommandS::CommandS():UndoableCommand{83}{}

std::vector<std::unique_ptr<Event>> enterInsertMode(const UndoableCommand *command, Window *w, const int &initShift, const bool &replace, const int &newline){
    std::vector<std::unique_ptr<Event>> events;
    if(replace) w->showStatus("-- REPLACE --");
    else w->showStatus("-- INSERT --");
    w->setState(STATE_INSERT);
    w->getCursor()->moveX(initShift);
    int ch;
    bool inserted = false;
    std::unique_ptr<Store> preStore;
    int cursorY, cursorX;
    std::stack<char> removed;
    std::vector<int> triggered;
    if(newline != 0){
        preStore = std::make_unique<Store>(*(w->getStore()));
        cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
        cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
        inserted = true;
        if (newline > 0){
            w->getCursor()->moveLineEnd();
            w->getCursor()->moveX(1);
            w->getCursor()->insert(10);
        } else {
            w->getCursor()->moveLineBegin();
            w->getCursor()->insert(10);
            w->getCursor()->moveY(-1);
        }
        w->render();
    }
    while ((ch = w->getKeyListener()->get())) {
        if (ch == 27){ triggered.push_back(27); break; }  // escape
        else if (ch >= 258 && ch <= 261 ){
            if(inserted){
                triggered.push_back(27);
                events.push_back(std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX, triggered));
                triggered.clear();
                inserted = false;
                preStore = nullptr;
                while(!removed.empty()) removed.pop();
            }
            if (ch == 259) w->getCursor()->moveY(-1);
            else if (ch == 258) w->getCursor()->moveY(1);
            else if (ch == 260) w->getCursor()->moveX(-1);
            else if (ch == 261) w->getCursor()->moveX(1);
        }
        else if (ch == 410) w->resize();
        else {
            triggered.push_back(ch);
            if(!inserted){
                preStore = std::make_unique<Store>(*(w->getStore()));
                cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
                cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
                inserted = true;
            }
            if (ch == 8 || ch == 263){
                if(replace){
                    bool restored = false;
                    if(!removed.empty()){
                        restored = true;
                        w->getCursor()->erase();
                        if(removed.top()){
                            w->getCursor()->insert(removed.top());
                            restored = false;
                        }
                        removed.pop();
                    }
                    if (w->getCursor()->isAtLineBegin()){
                        if(!restored) w->getCursor()->moveOne(-1);
                        w->getCursor()->moveX(1);
                    } else{
                        if(!restored) w->getCursor()->moveOne(-1);
                    }
                } else {
                    w->getCursor()->erase();
                }
            } else{
                if(replace){
                    removed.push(w->getCursor()->currChar());
                    if(removed.top()){
                        w->getCursor()->moveX(1);
                        w->getCursor()->erase();
                    }
                    w->getCursor()->insert(ch);
                }
                else{
                    w->getCursor()->insert(ch);
                }
            }
            w->render();
        }
    }
    w->setState(STATE_NORMAL);
    w->getCursor()->moveX(-1);
    w->showStatus("");

    if(inserted) events.push_back(std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX, triggered));
    else if(preStore) preStore.reset();

    return events;
}
std::vector<std::unique_ptr<Event>> enterInsertMode(const UndoableCommand *command, Window *w, const int &initShift){
    return enterInsertMode(command, w, initShift, false, 0);
}
std::vector<std::unique_ptr<Event>> enterInsertMode(const UndoableCommand *command, Window *w, const int &initShift, const bool &replace){
    return enterInsertMode(command, w, initShift, replace, 0);
}


void restoreStore(Window *w, StoreChangeEvent* storeChangeEvent){
    if(storeChangeEvent){
        w->getUniqueStore().swap(storeChangeEvent->getPreStore());
        std::list<std::string>::iterator &itLst = w->getCursor()->getItLst();
        std::string::iterator &itStr = w->getCursor()->getItStr();
        itLst = w->getStore()->getStrs().begin();
        for(int i = 0; i < storeChangeEvent->getCursorY(); ++i) ++itLst;
        itStr = w->getCursor()->getItLst()->begin();
        if (itLst->length() != 0) itStr += clampReturn(storeChangeEvent->getCursorX(), 0, itLst->length()-1);
        w->getCursor()->adjust();
        w->render();
    }
}

std::vector<std::unique_ptr<Event>> Commandi::runEvent(Window *w) const{ return enterInsertMode(this, w, 0); }
void Commandi::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> CommandI::runEvent(Window *w) const{ w->getCursor()->moveLineBeginNonWs(); return enterInsertMode(this, w, 0); }
void CommandI::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> Commanda::runEvent(Window *w) const{ return enterInsertMode(this, w, 1); }
void Commanda::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> CommandA::runEvent(Window *w) const{ w->getCursor()->moveLineEnd(); return enterInsertMode(this, w, 1); }
void CommandA::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> deleteOne(const UndoableCommand *command, Window *w, const int &shift){
    std::vector<std::unique_ptr<Event>> events;
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
        events.push_back(std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    } else{
        preStore.reset();
    }
    return events;
}

std::vector<std::unique_ptr<Event>> Commandx::runEvent(Window *w) const{ return deleteOne(this, w, 1); }
void Commandx::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> CommandX::runEvent(Window *w) const{ return deleteOne(this, w, 0); }
void CommandX::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::vector<std::unique_ptr<Event>> Commandr::runEvent(Window *w) const{
    int ch = w->getKeyListener()->get();
    std::vector<std::unique_ptr<Event>> events;
    if(!w->getCursor()->currChar()) return events;
    w->setState(STATE_INSERT);
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    w->getCursor()->moveX(1);
    w->getCursor()->erase();
    w->getCursor()->insert(ch);
    w->getCursor()->moveX(-1);
    w->render();
    w->setState(STATE_NORMAL);
    events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{ch}));
    return events;
}
void Commandr::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> CommandR::runEvent(Window *w) const{ return enterInsertMode(this, w, 0, true); }
void CommandR::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::vector<std::unique_ptr<Event>> CommandJ::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());

    std::list<std::string>::iterator &itLst = w->getCursor()->getItLst();
    std::string::iterator &itStr = w->getCursor()->getItStr();
    ++itLst;
    if(itLst != w->getStore()->getStrs().end()){
        std::string currLineContent = *itLst;
        itLst = w->getStore()->getStrs().erase(itLst);
        --itLst;
        int length = itLst->length();
        if(*(itLst->end()-1) != ' ') (*itLst) += " ";
        (*itLst) += trim(currLineContent);
        itStr = itLst->begin() + length;
        w->getCursor()->adjust();
        w->render();
        events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    } else{
        --itLst;
    }
    return events;
}
void CommandJ::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> Commando::runEvent(Window *w) const{ return enterInsertMode(this, w, 0, false, 1); }
void Commando::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
std::vector<std::unique_ptr<Event>> CommandO::runEvent(Window *w) const{ return enterInsertMode(this, w, 0, false, -1); }
void CommandO::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::vector<std::unique_ptr<Event>> CommandColon::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    set_escdelay(1000);
    w->getCursor()->setState(STATE_STATUS);
    std::string commandSymbol = ":";
    std::string commandString;
    w->showStatus(commandSymbol);
    w->getCursor()->moveTo(w->getMaxY()-1, 1);
    int ch;
    while ((ch = w->getKeyListener()->get())){
        if(ch == 10){
            if(commandString == "q!"){
                w->setRunning(false);
            } else if ((commandString + " ").substr(0, 2) == "w "){
                bool nameChanged = false;
                if(w->getParser()->getFileName().empty()){
                    std::string fileName = (commandString + " ").substr(2, commandString.length()-2);
                    if (fileName.empty()) w->showStatus("E32: No file name", STATE_ERROR);
                    else{ w->getParser()->setFileName(fileName); nameChanged = true; }
                }
                if(!w->getParser()->getFileName().empty()){
                    if(!w->getSaver()->save()){
                        w->showStatus("E0: Failed to save", STATE_ERROR);
                    }else{
                        w->showStatus("");
                        if(nameChanged) w->render();
                    }
                }
            } else if ((commandString + " ").substr(0, 3) == "wq "){
                if(w->getParser()->getFileName().empty()){
                    std::string fileName = (commandString + " ").substr(3, commandString.length()-3);
                    if (fileName.empty()) w->showStatus("E32: No file name", STATE_ERROR);
                    else w->getParser()->setFileName(fileName);
                }
                if(!w->getParser()->getFileName().empty()){
                    if(!w->getSaver()->save()){
                        w->showStatus("E0: Failed to save", STATE_ERROR);
                    }else{
                        w->setRunning(false);
                    }
                }
            } else if (commandString == "q"){
                if (!w->getSaver()->getModified()) w->setRunning(false);
                else w->showStatus("E37: No write since last change (add ! to override)", STATE_ERROR);
            } else if (commandString == "r"){
                w->getCursor()->setState(STATE_EDIT);
                std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
                int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
                int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
                std::list<std::string> strs = w->getStore()->getStrs();
                w->getStore()->getStrs().insert(std::next(w->getCursor()->getItLst(), 1), strs.begin(), strs.end());
                w->render();
                w->getCursor()->adjust();
                w->getCursor()->moveY(1);
                w->getCursor()->moveLineBegin();
                w->showStatus("");
                events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
            } else if (commandString == "0"){
                w->getCursor()->setState(STATE_EDIT);
                w->getCursor()->moveLineBegin();
                int currYDis = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
                for(int i = 0; i < currYDis; ++i) w->getCursor()->moveY(-1);
                w->showStatus("");
            } else if (commandString == "$"){
                w->getCursor()->setState(STATE_EDIT);
                w->getCursor()->moveLineBegin();
                int currYDis = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
                int totalLine = w->getStore()->getStrs().size();
                for(int i = currYDis; i < totalLine; ++i) w->getCursor()->moveY(1);
                w->showStatus("");
            } else if (isNumber(commandString)){
                w->getCursor()->setState(STATE_EDIT);
                w->getCursor()->moveLineBegin();
                long currLine = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst()) + 1;
                long targetLine = atoi(commandString.c_str());
                while(currLine != targetLine){
                    if(currLine > targetLine){
                        w->getCursor()->moveY(-1);
                        --currLine;
                    } else {
                        w->getCursor()->moveY(1);
                        ++currLine;
                    }
                }
                w->showStatus("");
            } else {
                w->showStatus("E492: Not an editor command: " +  commandString, STATE_ERROR);
            }
            break;
        }
        else if(ch == 27){w->showStatus(""); break;}
        else if (ch == 410) w->resize();
        else{
            if(ch == 8 || ch == 263){
                if(commandString.empty()) {w->showStatus(""); break;}
                commandString = commandString.substr(0, commandString.length()-1);
            } else {
                commandString += std::string(1, ch);
            }
            w->showStatus(commandSymbol + commandString);
            w->getCursor()->moveTo(w->getMaxY()-1, commandString.length() + 1);
        }
    }
    w->getCursor()->setState(STATE_EDIT);
    w->refreshCursor();
    set_escdelay(0);
    return events;
}
void CommandColon::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }



std::vector<std::unique_ptr<Event>> CommandAt::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;

    int ch = w->getKeyListener()->get();
    if(w->getKeyListener()->getMacros().find(ch) != w->getKeyListener()->getMacros().end()){
        std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
        int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
        int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
        w->getKeyListener()->stream(w->getKeyListener()->getMacros()[ch], w);
        events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    }

    return events;
}
void CommandAt::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::vector<std::unique_ptr<Event>> CommandDot::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    if(!w->getKeyListener()->getEventHistory().empty()){
        return w->getKeyListener()->getEventHistory().top()->reExecute(w);
    }
    return events;
}
void CommandDot::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }


std::vector<std::unique_ptr<Event>> cut(const UndoableCommand *command, Window *w, const bool &lastLine, const int &commandInt){
    std::vector<std::unique_ptr<Event>> events;
    int ch = w->getKeyListener()->get();
    std::vector<int> movement{ch};
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());

    if(ch == commandInt){
        w->getKeyListener()->getRegisters()[0] = *(w->getCursor()->getItLst()) + "\n";
        if(lastLine){
            *(w->getCursor()->getItLst()) = "";
            w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin();
        }else{
            w->getCursor()->getItLst() = w->getStore()->getStrs().erase(w->getCursor()->getItLst());
            if(w->getCursor()->getItLst() == w->getStore()->getStrs().end()) --(w->getCursor()->getItLst());
            w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin();
            if(w->getCursor()->getItLst() == w->getStore()->getStrs().begin()) w->getStore()->getItCurrY() = w->getStore()->getStrs().cbegin();
        }
        w->getCursor()->adjust();
    }else{
        std::list<std::string>::iterator initItLst = w->getCursor()->getItLst();
        std::string::iterator initItStr = w->getCursor()->getItStr();
        w->setState(STATE_INSERT);
        w->getKeyListener()->stream(movement, w);
        if(w->getCursor()->getItStr() == initItStr){ preStore.reset(); return events; }
        if(w->getCursor()->getItLst() == initItLst){
            int distance = w->getCursor()->getItStr() - initItStr;
            if(distance > 0){
                w->getKeyListener()->getRegisters()[0] = std::string(initItStr, w->getCursor()->getItStr());
                for(int i = 0; i < distance; ++i){ w->getCursor()->erase(); }
            }else{
                w->getKeyListener()->getRegisters()[0] = std::string(w->getCursor()->getItStr(), initItStr);
                w->getCursor()->getItStr() = initItStr;
                for(int i = 0; i < (-distance); ++i){ w->getCursor()->erase(); }
                w->getCursor()->moveX(1);
            }
        }else{
            int distance = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst()) - std::distance(w->getStore()->getStrs().begin(), initItLst);
            std::string res;

            if (distance > 0){
                while(w->getCursor()->getItLst() != initItLst){
                    res = *(w->getCursor()->getItLst()) + "\n" + res;
                    w->getCursor()->getItLst() = w->getStore()->getStrs().erase(w->getCursor()->getItLst());
                    --(w->getCursor()->getItLst());
                }
                if(lastLine){
                    res = *(initItLst) + "\n" + res;
                    *(initItLst) = "";
                }else{
                    res = *(w->getCursor()->getItLst()) + "\n" + res;
                    w->getCursor()->getItLst() = w->getStore()->getStrs().erase(w->getCursor()->getItLst());
                    if(w->getCursor()->getItLst() == w->getStore()->getStrs().begin()) w->getStore()->getItCurrY() = w->getStore()->getStrs().cbegin();
                    if(w->getCursor()->getItLst() == w->getStore()->getStrs().end()) --(w->getCursor()->getItLst());
                }
            }else{
                while(w->getCursor()->getItLst() != initItLst){
                    res += *(w->getCursor()->getItLst()) + "\n";
                    w->getCursor()->getItLst() = w->getStore()->getStrs().erase(w->getCursor()->getItLst());
                }
                if(w->getCursor()->getItLst() == w->getStore()->getStrs().begin()) w->getStore()->getItCurrY() = w->getStore()->getStrs().cbegin();
                if(lastLine){
                    res = *(initItLst) + "\n" + res;
                    *(initItLst) = "";
                }else{
                    res = *(w->getCursor()->getItLst()) + "\n" + res;
                    w->getCursor()->getItLst() = w->getStore()->getStrs().erase(w->getCursor()->getItLst());
                    if(w->getCursor()->getItLst() == w->getStore()->getStrs().begin()) w->getStore()->getItCurrY() = w->getStore()->getStrs().cbegin();
                    if(w->getCursor()->getItLst() == w->getStore()->getStrs().end()) --(w->getCursor()->getItLst());
                }
            }
            w->getKeyListener()->getRegisters()[0] = res;
            w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin();
            w->getCursor()->adjust();
        }
    }
    w->setState(STATE_NORMAL);
    w->render();
    events.push_back(std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX, movement));
    if(lastLine){
        for(auto &it : enterInsertMode(command, w, 0)){
            events.push_back(std::move(it));
        }
    }

    return events;
}


std::vector<std::unique_ptr<Event>> Commandc::runEvent(Window *w) const{ return cut(this, w, true, 99); }
void Commandc::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> Commandd::runEvent(Window *w) const{ return cut(this, w, false, 100); }
void Commandd::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> Commandy::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events = cut(this, w, false, 121);
    restoreStore(w, dynamic_cast<StoreChangeEvent*>(events.at(0).get()));
    events.at(0).reset();
    events.clear();
    return events;
}
void Commandy::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> paste(const UndoableCommand *command, Window *w, const int &direction){
    std::vector<std::unique_ptr<Event>> events;
    if(w->getKeyListener()->getRegisters().find(0) == w->getKeyListener()->getRegisters().end()) return events;
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    std::stringstream ss{w->getKeyListener()->getRegisters()[0]};
    ss >> std::noskipws;
    char c;
    w->setState(STATE_INSERT);
    if(w->getKeyListener()->getRegisters()[0].find("\n") == std::string::npos) w->getCursor()->moveX(direction);
    else{ w->getCursor()->moveY(direction); w->getCursor()->moveLineBegin(); }
    int line = 0;
    while (ss >> c){
        if (c == 10) ++line;
        w->getCursor()->insert(c);
    }
    for(int i = 0; i < line; ++i) w->getCursor()->moveY(-direction);
    w->getCursor()->moveX(-1);
    w->render();
    w->setState(STATE_NORMAL);
    events.push_back(std::make_unique<StoreChangeEvent>(command, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    return events;
}

std::vector<std::unique_ptr<Event>> Commandp::runEvent(Window *w) const{ return paste(this, w, 1); }
void Commandp::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> CommandP::runEvent(Window *w) const{ return paste(this, w, 0); }
void CommandP::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> Commands::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    w->setState(STATE_INSERT);
    w->getCursor()->moveX(1);
    w->getCursor()->erase();
    w->render();
    for(auto &it : enterInsertMode(this, w, 0)){
        events.push_back(std::move(it));
    }
    return events;
}
void Commands::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }

std::vector<std::unique_ptr<Event>> CommandS::runEvent(Window *w) const{
    std::vector<std::unique_ptr<Event>> events;
    std::unique_ptr<Store> preStore = std::make_unique<Store>(*(w->getStore()));
    int cursorY = std::distance(w->getStore()->getStrs().begin(), w->getCursor()->getItLst());
    int cursorX = std::distance((w->getCursor()->getItLst())->begin(), w->getCursor()->getItStr());
    events.push_back(std::make_unique<StoreChangeEvent>(this, std::move(preStore), cursorY, cursorX, std::vector<int>{}));
    *(w->getCursor()->getItLst()) = "";
    w->getCursor()->getItStr() = w->getCursor()->getItLst()->begin();
    w->getCursor()->adjust();
    w->render();
    for(auto &it : enterInsertMode(this, w, 0)){
        events.push_back(std::move(it));
    }
    return events;
}
void CommandS::reverseExecute(Window *w, Event *e) const{ restoreStore(w, dynamic_cast<StoreChangeEvent*>(e)); }
