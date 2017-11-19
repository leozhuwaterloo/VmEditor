#include "window.h"
#include <ncurses.h>
#include <fstream>
#include <algorithm>
#include "keylistener.h"
#include "command.h"
#include "utils.h"

Window::Cursor::Cursor(int y, int x, Window *window):y{y}, x{x}, window{window}{}
void Window::Cursor::moveTo(const int &y, const int &x){
    this->y = y;
    this->x = x;
    clamp(this->x, 0, window->getMaxX()-1);
    clamp(this->y, 0, window->getMaxY()-1);
    window->refreshCursor();
}
void Window::Cursor::moveY(const int &y){
    this->y += y;
    clamp(this->y, 0, window->getMaxY()-1);
    window->refreshCursor();
}
void Window::Cursor::moveX(const int &x){
    this->x += x;
    clamp(this->x, 0, window->getMaxX()-1);
    window->refreshCursor();
}
const int Window::Cursor::getY() const{ return y; }
const int Window::Cursor::getX() const{ return x; }


Window::Window(std::string fileName, std::unique_ptr<KeyListener> keyListener):fileName{fileName}, keyListener{std::move(keyListener)}{
    initscr();
    noecho();
    getmaxyx(stdscr, maxY, maxX);
    cursor = std::make_unique<Cursor>(0, 0, this);
    if(fileName.empty()){

    }else {
        std::fstream f {fileName};
        content.assign((std::istreambuf_iterator<char>(f)),(std::istreambuf_iterator<char>()));
    }
}

Window::~Window(){
    endwin();
}

KeyListener* Window::getKeyListener(){
    return keyListener.get();
}

void Window::init(){
    render();
    keyListener->init(this);
}

void Window::render(){
    for(int i = 1; i < maxY - 1; ++i){
        mvprintw(i, 0, "~");
    }
    move(0,0);

    printw(content.c_str());
    refreshCursor();
    refresh();
}

void Window::refreshCursor(){
    move(cursor->getY(), cursor->getX());
}

void Window::showStatus(const std::string &status){
    mvprintw(maxY-1, 0, status.c_str());
    refreshCursor();
    refresh();
}


const int Window::getMaxY() const{ return maxY; }
const int Window::getMaxX() const{ return maxX; }
Window::Cursor* Window::getCursor() { return cursor.get(); }
