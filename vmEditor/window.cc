#include "window.h"
#include <ncurses.h>
#include "keylistener.h"
#include "colormanager.h"
#include "highlightergroup.h"
#include "highlighter.h"
#include "command.h"
#include "parser.h"
#include "store.h"
#include "utils.h"
#include <stdlib.h>

Window::Cursor::Cursor(int y, int x, Window *window):y{y}, x{x}, xLoss{0}, preX{x}, window{window}{}
void Window::Cursor::init(Store *store){
    itLst = store->getStrs().begin();
    itStr = itLst->begin();
}
void Window::Cursor::adjust(){
    int newX = 0;
    int newY = 0;
    xLoss = 0;
    std::list<std::string>::const_iterator outerIt = window->getStore()->getItCurrY();

    while (outerIt != itLst) {
        newY += outerIt->length() / window->getMaxX() + 1;
        if (newY >= window->getMaxY() - window->getStore()->getNumInvalid()) break;
        ++outerIt;
    }

    if (outerIt == itLst) {
        std::string::const_iterator innerIt= outerIt->begin();
        while (innerIt != itStr) {
            if (newX == window->getMaxX()) {
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
    } else {
        y = window->getMaxY() - 1 - window->getStore()->getNumInvalid();
        xLoss = outerIt->length() / window->getMaxX();
        preX = outerIt->length();
        x = preX - xLoss * window->getMaxX();
    }
}
void Window::Cursor::moveTo(const int &y, const int &x){
    this->y = y;
    this->x = x;
    clamp(this->x, 0, window->getMaxX()-1);
    clamp(this->y, 0, window->getMaxY()-1);
    window->refreshCursor();
}
void Window::Cursor::moveY(const int &y){
    if(itLst != window->getStore()->getStrs().end() && std::next(itLst, y) != window->getStore()->getStrs().end() && (this->y + y + window->getStore()->getCurrY()) >= 0){
        size_t yDiff = 0;
        for(int i = 0; i < std::abs(y); ++i){
            if (y < 0){
                --itLst;
                yDiff -= itLst->length() / window->getMaxX() + 1 + xLoss;
             }else if (y > 0){
                yDiff += itLst->length() / window->getMaxX() + 1 - xLoss;
                ++itLst;
            }
        }
        this->y += yDiff;
        this->x = preX;
        clamp(this->x, 0, itLst->length()-1);
        if(this->x == -1) this->x = 0; //for empty string
        itStr = itLst->begin() + x;
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
    }
}
void Window::Cursor::moveX(const int &x){
    int nextX = clampReturn(preX, 0, itLst->length()-1) + x;

    if(clampReturn(nextX, 0, itLst->length()-1) == nextX){
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
        preX = window->getMaxX() * xLoss + this->x;
        window->refreshCursor();
    }
}
const int Window::Cursor::getY() const{ return y; }
const int Window::Cursor::getX() const{ return x; }


Window::Window(std::unique_ptr<KeyListener> keyListener,
    std::unique_ptr<ColorManager> colorManager, std::unique_ptr<Parser> parser):
    keyListener{std::move(keyListener)}, colorManager{std::move(colorManager)}, parser{std::move(parser)}{
    initscr();
    noecho();
    getmaxyx(stdscr, maxY, maxX);
    cursor = std::make_unique<Cursor>(0, 0, this);
}

Window::~Window(){
    endwin();
}

void Window::init(const std::string &fileName){
    store = std::move(parser->parse(fileName));
    cursor->init(store.get());
    colorManager->init(parser->getFileName());
    render();
    keyListener->init(this);
}

void Window::render(){
    for(int i = 1; i < maxY - 1; ++i){
        colorManager->mvprintColor(i, 0, "~", COLOR_BLUE);
    }
    colorManager->mvprint(0, 0, store->getRenderString(maxY, maxX));
    for(int i = 0; i < store->getNumInvalid(); ++i){
        colorManager->printColor("@\n", COLOR_BLUE);
    }
    refreshCursor();
    refresh();
}

void Window::resize(){
    getmaxyx(stdscr, maxY, maxX);
    cursor->adjust();
    render();
}

void Window::refreshCursor(){
    move(cursor->getY(), cursor->getX());
}

void Window::showStatus(const std::string &status){
    colorManager->mvprint(maxY-1, 0, status + "\n");
    refreshCursor();
    refresh();
}


const int Window::getMaxY() const{ return maxY; }
const int Window::getMaxX() const{ return maxX; }
Window::Cursor* Window::getCursor() { return cursor.get(); }
Store* Window::getStore() { return store.get(); }
KeyListener* Window::getKeyListener(){ return keyListener.get(); }
ColorManager* Window::getColorManager(){ return colorManager.get(); }
