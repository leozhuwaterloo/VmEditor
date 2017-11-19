#include "window.h"
#include "keylistener.h"
#include "command.h"
#include <ncurses.h>
#include <fstream>

Window::Window(std::string fileName, std::unique_ptr<KeyListener> keyListener):fileName{fileName}, keyListener{std::move(keyListener)}{
    initscr();
    getmaxyx(stdscr, y, x);
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
    keyListener->init(this);
    render();
}

void Window::render(){
    for(int i = 0; i < y - 1; ++i){
        mvprintw(i, 0, "~");
    }
    move(0,0);

    printw(content.c_str());
    refresh();
}
