#include "window.h"
#include "keylistener.h"
#include "command.h"
#include <memory>

void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<CommandInsert>());
}
