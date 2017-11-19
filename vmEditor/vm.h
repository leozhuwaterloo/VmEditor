#include "window.h"
#include "keylistener.h"
#include "command.h"
#include <memory>

void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<CommandInsert>());
    keyListener->addCommand(std::make_unique<CommandUp>());
    keyListener->addCommand(std::make_unique<CommandDown>());
    keyListener->addCommand(std::make_unique<CommandLeft>());
    keyListener->addCommand(std::make_unique<CommandRight>());
}
