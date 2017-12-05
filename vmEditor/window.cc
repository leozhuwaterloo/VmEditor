#include "window.h"
#include <ncurses.h>
#include "keylistener.h"
#include "colormanager.h"
#include "highlightergroup.h"
#include "highlighter.h"
#include "command.h"
#include "event.h"
#include "parser.h"
#include "saver.h"
#include "store.h"
#include "utils.h"
#include <signal.h>
#include <stdlib.h>
#include <locale>

Window::Cursor::Cursor(int y, int x, Window *window):y{y}, x{x}, nLine{y+1}, nChar{x+1}, xLoss{0}, preX{x}, window{window}, cursorState{STATE_EDIT}{}
void Window::Cursor::init(Store *store){
    itLst = store->getStrs().begin();
    itStr = itLst->begin();
}
void Window::Cursor::adjust(){
    int newX = 0;
    int newY = 0;
    xLoss = 0;
    std::list<std::string>::const_iterator outerIt = window->getStore()->getItCurrY();

    // so that it is guaranteed to find it
    while (outerIt != itLst) {
        newY += outerIt->length() / window->getMaxX() + 1;
        ++outerIt;
    }

    // readjust cursor's position if it is out of bound
    int firstLineLength;
    int currLineLength = outerIt->length() / window->getMaxX() + 1;
    bool storeMoved = false;
    while (newY + currLineLength > window->getMaxY() - 1 - window->getStore()->getNumInvalid()) {
        firstLineLength = window->getStore()->getItCurrY()->length() / window->getMaxX() + 1;
        window->getStore()->moveY(1);
        storeMoved = true;
        newY-=firstLineLength;
        window->showStatus(std::to_string(newY));
    }
    if(storeMoved) window->render();

    std::string::const_iterator innerIt= outerIt->begin();
    while (innerIt != itStr) {
        if (newX == window->getMaxX()-1) {
            ++newY;
            ++xLoss;
            newX = 0;
        } else {
            ++newX;
        }
        ++innerIt;
    }
    y = newY;
    x = newX;
    preX = window->getMaxX() * xLoss + x;
}
void Window::Cursor::moveTo(const int &y, const int &x){
    if(cursorState == STATE_STATUS){
        this->statusY = y;
        this->statusX = x;
        clampReturn(this->statusY, 0, window->getMaxY()-1);
        clampReturn(this->statusX, 0, window->getMaxX()-1);
        window->refreshCursor();
    }
}
void Window::Cursor::moveY(const int &y){
    if(cursorState == STATE_STATUS) return;
    if(itLst != window->getStore()->getStrs().end() && std::next(itLst, y) != window->getStore()->getStrs().end() && (this->y + y + window->getStore()->getCurrY()) >= 0){
        size_t yDiff = 0;
        for(int i = 0; i < std::abs(y); ++i){
            if (y < 0){
                --itLst;
                --nLine;
                yDiff -= itLst->length() / window->getMaxX() + 1 + xLoss;
            }else if (y > 0){
                yDiff += itLst->length() / window->getMaxX() + 1 - xLoss;
                ++itLst;
                ++nLine;
            }
        }
        this->y += yDiff;
        this->x = preX;
        clamp(this->x, 0, itLst->length()-1+window->getStateLineEnd());
        nChar = this->x + 1;
        if(this->x == -1) this->x = 0; //for empty string
        itStr = itLst->begin() + this->x;
        xLoss = 0;
        //move on if necessary
        while(this->x >= window->getMaxX()){
            ++xLoss;
            this->x -= window->getMaxX();
            ++this->y;
        }

        //move on the store
        int currLineLength, firstLineLength, counter = 0;
        if(this->y >= window->getMaxY() - 1 - window->getStore()->getNumInvalid()){
            currLineLength = itLst->length() / window->getMaxX() + 1;
            while(currLineLength - window->getStore()->getNumInvalid() > 0){
                firstLineLength = window->getStore()->getItCurrY()->length() / window->getMaxX() + 1;
                window->getStore()->moveY(1);
                currLineLength-=firstLineLength;
                counter+=firstLineLength;
            }
            this->y -= counter;
            window->render();
        } else if (this->y < 0){
            window->getStore()->moveY(-1);
            currLineLength = itLst->length() / window->getMaxX() + 1;
            this->y += currLineLength;
            window->render();
        }

        window->refreshCursor();
        window->showStatus();
    }
}
void Window::Cursor::moveX(const int &x){
    if(cursorState == STATE_STATUS) return;
    int xBound = itLst->length()-1+window->getStateLineEnd();
    int nextX = clampReturn(preX, 0, xBound) + x;
    if(window->getStateLineEnd() == 1 && preX >= xBound && currChar()){
        nextX = 0;
    }
    if(clampReturn(nextX, 0, xBound) == nextX){
        while((this->x + x) >= window->getMaxX()){
            ++xLoss;
            this->x -= window->getMaxX();
            ++y;
        }
        while((this->x + x) < 0){
            --xLoss;
            this->x += window->getMaxX();
            --y;
        }

        this->x += x;
        itStr += x;
        nChar += x;
        preX = window->getMaxX() * xLoss + this->x;
        window->refreshCursor();
        window->showStatus();
    }
}
void Window::Cursor::moveOne(const int &n) {
    if(cursorState == STATE_STATUS) return;
    if (n > 0) {
        if (isAtEnd()) return;
        int oldX = x;
        moveX(1);
        if (x == oldX) {
            moveY(1);
            moveLineBegin();
        }
    } else if (n < 0) {
        if (isAtBegin()) return;
        int oldX = x;
        moveX(-1);
        if (x == oldX) {
            moveY(-1);
            moveLineEnd();
        }
    }
}
void Window::Cursor::moveLineBegin() { if(cursorState == STATE_STATUS) return; while (!isAtLineBegin()) moveX(-1); }
void Window::Cursor::moveLineEnd() { if(cursorState == STATE_STATUS) return; while (!isAtLineEnd()) moveX(1); }
void Window::Cursor::moveLineBeginNonWs() {
    if(cursorState == STATE_STATUS) return;
    moveLineBegin();
    while (!isAtLineEnd() && std::isspace(currChar())) moveX(1);
}
void Window::Cursor::insert(char c) {
    if(cursorState == STATE_STATUS) return;
    if (c == 10){ //enter
        std::string first_half = std::string(itLst->begin(), itStr);
        std::string second_half = std::string(itStr, itLst->end());
        (*itLst) = first_half;
        ++itLst;
        ++nLine;
        itLst = window->getStore()->getStrs().insert(itLst, second_half);
        itStr = itLst->begin();
        nChar = 0;
        adjust(); //too tired to writing another logic just call adjust
        window->showStatus();
    }else if(c == 9){
        for(int i = 0; i< 4; ++i){
            itStr = itLst->insert(itStr, ' ');
            moveX(1);
        }
    }else{
        itStr = itLst->insert(itStr, c);
        moveX(1);
    }
}
void Window::Cursor::erase() {
    if(cursorState == STATE_STATUS) return;
    if(isAtLineBegin()){
        if(!isAtBegin()){
            std::string currLineContent = *itLst;
            itLst = window->getStore()->getStrs().erase(itLst);
            --itLst;
            --nLine;
            int length = itLst->length();
            (*itLst) += currLineContent;
            itStr = itLst->begin() + length;
            nChar = length;
            adjust();
            window->showStatus();
        }
    }else{
        moveX(-1);
        itStr = itLst->erase(itStr);
    }
}
const int Window::Cursor::getY() const{ if (cursorState == STATE_EDIT) return y; else return statusY; }
const int Window::Cursor::getX() const{ if (cursorState == STATE_EDIT) return x; else return statusX; }
std::list<std::string>::iterator &Window::Cursor::getItLst() { return itLst; }
std::string::iterator &Window::Cursor::getItStr() { return itStr; }
const int Window::Cursor::getNLine() const{ return nLine; }
const int Window::Cursor::getNChar() const{ return nChar; }
const int Window::Cursor::currChar() const{ return *itStr; }
bool Window::Cursor::isAtEmptyLine() const { return itLst->length() == 0; }
bool Window::Cursor::isAtBegin() const { return itStr == window->getStore()->getStrs().begin()->begin(); }
bool Window::Cursor::isAtEnd() const { return itStr + 1 == std::next(window->getStore()->getStrs().end(), -1)->end(); }
bool Window::Cursor::isAtLineBegin() const { return itStr == itLst->begin(); }
bool Window::Cursor::isAtLineEnd(const int &offset) const { return itLst->length() == 0 || itStr + 1 - offset == itLst->end(); }
bool Window::Cursor::isAtLineEnd() const { return isAtLineEnd(0); }
void Window::Cursor::setState(CursorState cursorState){ this->cursorState = cursorState; }

Window::Window(std::unique_ptr<KeyListener> keyListener,
    std::unique_ptr<ColorManager> colorManager, std::unique_ptr<Parser> parser,  std::unique_ptr<Saver> saver):
    keyListener{std::move(keyListener)}, colorManager{std::move(colorManager)}, parser{std::move(parser)}, saver{std::move(saver)},
    state{STATE_NORMAL}, running{true}{

    initscr();
    noecho();
    keypad(stdscr, true);
    set_escdelay(0);
    signal(SIGINT, SIG_IGN);
    getmaxyx(stdscr, maxY, maxX);
    cursor = std::make_unique<Cursor>(0, 0, this);
}

Window::~Window(){
    endwin();
}

void Window::init(const std::string &fileName){
    store = std::move(parser->parse(fileName));
    saver->init(parser.get(), store.get());
    cursor->init(store.get());
    colorManager->init(parser.get());
    if(!parser->isNewFile()) showStatus("\""+ fileName + "\" " + std::to_string(parser->getlCount()) + "L, " + std::to_string(parser->getcCount()) + "C");
    else if(parser->isNewFile() && !parser->getFileName().empty())  showStatus("\""+ fileName + "\" [Newfile]");
    render();
    keyListener->init(this);
}

void Window::render(){
    for(int i = 1; i < maxY - 1; ++i){
        colorManager->mvprintColor(i, 0, "~\n", COLOR_CYAN);
    }
    colorManager->mvprint(0, 0, store->getRenderString(maxY, maxX));
    for(int i = 0; i < store->getNumInvalid(); ++i){
        colorManager->printColor("@\n", COLOR_CYAN);
    }
    refreshCursor();
    refresh();
}

void Window::resize(){
    getmaxyx(stdscr, maxY, maxX);
    cursor->adjust();
    render();
    showStatus();
}

void Window::refreshCursor(){
    move(cursor->getY(), cursor->getX());
}

void Window::showStatus(const StatusState &statusState){
    if(statusState == STATE_GENERAL) colorManager->mvprint(maxY-1, 0, status + keyListener->getRecordingStatus() + "\n", false);
    else colorManager->mvprintColor(maxY-1, 0, status + "\n", COLOR_WHITE, COLOR_RED);

    std::string nLine = std::to_string(cursor->getNLine());
    std::string nChar = cursor->getNChar() > 0 ? std::to_string(cursor->getNChar()) : "0-1";
    colorManager->mvprint(maxY-1, maxX-17-nLine.length(),  nLine + "," + nChar + "\n", false);
    colorManager->mvprint(maxY-1, maxX-4, "Top \n", false);
    refreshCursor();
    refresh();
}
void Window::showStatus(){ showStatus(STATE_GENERAL); }

void Window::showStatus(const std::string &status, const StatusState &statusState){
    this->status = status;
    showStatus(statusState);
}
void Window::showStatus(const std::string &status){showStatus(status, STATE_GENERAL); }

const int Window::getMaxY() const{ return maxY; }
const int Window::getMaxX() const{ return maxX; }
const int Window::getStateLineEnd() const{ if (state == STATE_INSERT) return 1; else return 0; }
void Window::setState(State state){ this->state = state; }
Window::Cursor* Window::getCursor() { return cursor.get(); }
Parser* Window::getParser(){ return parser.get(); }
Saver* Window::getSaver(){ return saver.get(); }
Store* Window::getStore() { return store.get(); }
std::unique_ptr<Store> &Window::getUniqueStore() { return store; }
KeyListener* Window::getKeyListener(){ return keyListener.get(); }
ColorManager* Window::getColorManager(){ return colorManager.get(); }
bool Window::isRunning() const{ return running; }
void Window::setRunning(const bool &running){ this->running = running; }
