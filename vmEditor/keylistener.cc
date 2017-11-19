#include "keylistener.h"
#include <ncurses.h>
#include <string>
#include "command.h"
#include "window.h"

void KeyListener::init(Window *window) {
    int c;
    while(c = getch()){
        currKeys.push_back(c);
        if(commands.find(currKeys) != commands.end()){
            commands[currKeys]->execute(window);
        }else{
            window->showStatus(std::to_string(c));
        }
        currKeys.clear();
    }
}

void KeyListener::addCommand(std::unique_ptr<Command> command) {
    commands[command->getKeys()] = std::move(command);
}
