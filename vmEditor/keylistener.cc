#include "keylistener.h"
#include <ncurses.h>
#include <string>
#include "command.h"
#include "event.h"
#include "window.h"

void KeyListener::init(Window *window) {
    int c;
    while(c = getch()){
        currKeys.push_back(c);
        if(commands.find(currKeys) != commands.end()){
            std::unique_ptr<Event> e = commands[currKeys]->execute(window);
            if (e != nullptr){
                eventHistory.push(std::move(e));
            }
        }else{
            window->showStatus(std::to_string(c));
        }
        currKeys.clear();
    }
}

void KeyListener::addCommand(std::unique_ptr<Command> command) {
    commands[command->getKeys()] = std::move(command);
}

std::stack<std::unique_ptr<Event>> &KeyListener::getEventHistory(){ return eventHistory; }
