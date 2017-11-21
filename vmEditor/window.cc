#include "window.h"
#include <ncurses.h>
#include "keylistener.h"
#include "colormanager.h"
#include "highlighter.h"
#include "command.h"
#include "parser.h"
#include "store.h"
#include "utils.h"
#include <stdlib.h>

Window::Cursor::Cursor(int y, int x, Window *window):y{y}, x{x}, window{window}{}
void Window::Cursor::init(Store *store){
    itLst = store->getStrs().begin();
    itStr = itLst->begin();
}
void Window::Cursor::adjust(){
    
}
void Window::Cursor::moveTo(const int &y, const int &x){
    this->y = y;
    this->x = x;
    clamp(this->x, 0, window->getMaxX()-1);
    clamp(this->y, 0, window->getMaxY()-1);
    window->refreshCursor();
}
void Window::Cursor::moveY(const int &y){
    if(itLst != window->getStore()->getStrs().end() && (this->y + y) >= 0){
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
        if(this->x == -1) this->x = 0;
        itStr = itLst->begin() + x;
        xLoss = 0;
        while(this->x >= window->getMaxX()){
            ++xLoss;
            this->x -= window->getMaxX();
            ++this->y;
        }
        window->refreshCursor();
    }
}
void Window::Cursor::moveX(const int &x){
    if(itStr != itLst->end() && (itStr + x) != itLst->end() && (this->x + x + window->getMaxX() * xLoss) >= 0){
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
    colorManager->init(parser->getFileType());
    render();
    keyListener->init(this);
}

void Window::render(){
    for(int i = 1; i < maxY - 1; ++i){
        colorManager->mvprintColor(i, 0, "~", COLOR_BLUE);
    }

    colorManager->mvprint(0, 0, store->getRenderString());
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
    colorManager->mvprint(maxY-1, 0, status);
    refreshCursor();
    refresh();
}


const int Window::getMaxY() const{ return maxY; }
const int Window::getMaxX() const{ return maxX; }
Window::Cursor* Window::getCursor() { return cursor.get(); }
Store* Window::getStore() { return store.get(); }
KeyListener* Window::getKeyListener(){ return keyListener.get(); }
ColorManager* Window::getColorManager(){ return colorManager.get(); }
