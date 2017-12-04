#include "keylistener.h"
#include <ncurses.h>
#include <string>
#include "command.h"
#include "event.h"
#include "window.h"
#include "saver.h"

KeyListener::KeyListener():searched{false}, recording{false}{}

void KeyListener::init(Window *window) {
    int c;
    while(window->isRunning() && (c = get())){
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
std::pair<char, int> &KeyListener::getLatestFind(){ return latestFind; }
bool KeyListener::getFinded() const{ return finded; }
void KeyListener::setLatestFind(const char &target, const int &direction){ latestFind = std::make_pair(target, direction); finded = true; }
std::stack<std::unique_ptr<Event>> &KeyListener::getEventHistory(){ return eventHistory; }
bool KeyListener::isRecording() const{ return recording; }
void KeyListener::startRecording(const int &targetReg){ recording = true; this->targetReg = targetReg; macros[targetReg].clear(); }
void KeyListener::stopRecording(){ recording = false; }
std::map<int, std::vector<int>> &KeyListener::getMacros(){ return macros; }
std::string KeyListener::getRecordingStatus() const{ if(recording) return "recording @" + std::string(1, targetReg); else return "";}


void KeyListener::pushStream(const std::vector<int> &stream){
    for(std::vector<int>::const_reverse_iterator it = stream.rbegin(); it != stream.rend(); ++it){
        trigger.push(*it);
    }
}

void KeyListener::stream(const std::vector<int> &stream, Window *window){
    std::vector<int> keys;
    pushStream(stream);

    while(!trigger.empty()){
        keys.push_back(get());
        if(commands.find(keys) != commands.end()){
            commands[keys]->execute(window);
        }
        keys.clear();
    }
}

int KeyListener::get(){
    int ch;
    if(!trigger.empty()){
        ch = trigger.top();
        trigger.pop();
    } else {
        ch = getch();
    }
    if (recording && ch != 113) macros[targetReg].push_back(ch);
    return ch;
}

std::map<int, std::string> &KeyListener::getRegisters(){ return registers; }
