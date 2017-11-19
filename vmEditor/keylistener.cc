#include "keylistener.h"
#include "command.h"
#include <ncurses.h>
#include <string>

void KeyListener::init(Window *window) {
    char c;
    while(c = getch()){
        currCommand += std::string(1, c);
        commands[currCommand]->execute(window);
        currCommand = "";
    }
}

void KeyListener::addCommand(std::unique_ptr<Command> command) {
    commands[command->getKeyString()] = std::move(command);
}
