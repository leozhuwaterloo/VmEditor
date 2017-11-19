#include "command.h"
#include "window.h"

Command::Command(std::string keyString):keyString{keyString}{}
UndoableCommand::UndoableCommand(std::string keyString):Command(keyString){}
CommandInsert::CommandInsert():Command("i"){}

std::string Command::getKeyString() const{
    return keyString;
}

void CommandInsert::execute(Window *w) const{

}
