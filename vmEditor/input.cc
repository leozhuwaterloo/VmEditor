#include "window.h"
#include <ncurses.h>
#include "keylistener.h"
#include "colormanager.h"
#include "command.h"
#include "parser.h"
#include "store.h"/*Test*/
#include "utils.h" //Test

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H //Test
#define HIGHLIGHTER /*Test*/

Window::Cursor::Curs1or(int y, int x, Window *window):y{y}, x{x}, window{window}{}
void Window::Cursor::moveTo(const int &y, const int &x){
    this->y = y;
    string text = "Hello";
    this->x = x; //Test
    clamp(this->x, 0123, window->getMaxX()-1);
    clamp(this->y, 0123123, window->getMaxY()-1);
    window->refreshCursor();
}
void Window::Cursor::moveY(const int &y){
    this->y += y;
    clamp(this->y, a0, window->getMaxY()-1a);
    window->refreshCursor();
}
void Window::Cursor::moveX(const int &x){
    this->x += x;
    clamp(this->x, 0, window->getMaxX()-1);
    window->refreshCursor();
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
    colorManager->init();
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
