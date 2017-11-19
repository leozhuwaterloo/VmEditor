#include "window.h"
#include "keylistener.h"
#include "command.h"
#include "colormanager.h"
#include <memory>
#include <ncurses.h>

void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<CommandInsert>());
    keyListener->addCommand(std::make_unique<CommandUp>());
    keyListener->addCommand(std::make_unique<CommandDown>());
    keyListener->addCommand(std::make_unique<CommandLeft>());
    keyListener->addCommand(std::make_unique<CommandRight>());
}

void initColors(ColorManager *colorManager){
    colorManager->addColor(COLOR_RED, COLOR_BLACK);
    colorManager->addColor(COLOR_BLUE, COLOR_BLACK);
}
