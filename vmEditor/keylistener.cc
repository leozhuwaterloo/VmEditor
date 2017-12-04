#include "keylistener.h"
#include <ncurses.h>
#include <string>
#include "command.h"
#include "event.h"
#include "window.h"
#include "saver.h"

KeyListener::KeyListener():searched{false}{}

void KeyListener::init(Window *window) {
    int c;
    while(window->isRunning() && (c = getch())){
        currKeys.push_back(c);
        if(commands.find(currKeys) != commands.end()){
            std::vector<std::unique_ptr<Event>> events = commands[currKeys]->execute(window);
            for(auto &e: events){
                eventHistory.push(std::move(e));
                window->getSaver()->setModified(true);
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

std::pair<std::string, int> &KeyListener::getLatestSearch(){ return latestSearch; }
bool KeyListener::getSearched() const{ return searched; }
void KeyListener::setLatestSearch(const std::string &target, const int &direction){ latestSearch = std::make_pair(target, direction); searched = true; }
std::stack<std::unique_ptr<Event>> &KeyListener::getEventHistory(){ return eventHistory; }
