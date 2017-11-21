#include "window.h"
#include "keylistener.h"
#include "command.h"
#include "colormanager.h"
#include "highlighter.h"
#include "parser.h"
#include <memory>
#include <ncurses.h>

void initCommands(KeyListener *keyListener);
void initColors(ColorManager *colorManagerr);
void initHighlighter(ColorManager *colorManagerr);


void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<CommandInsert>());
    keyListener->addCommand(std::make_unique<CommandUp>());
    keyListener->addCommand(std::make_unique<CommandDown>());
    keyListener->addCommand(std::make_unique<CommandLeft>());
    keyListener->addCommand(std::make_unique<CommandRight>());
    keyListener->addCommand(std::make_unique<CommandResize>());
}

void initColors(ColorManager *colorManager){
    colorManager->addColor(COLOR_WHITE, COLOR_BLACK);
    colorManager->addColor(COLOR_RED, COLOR_BLACK);
    colorManager->addColor(COLOR_BLUE, COLOR_BLACK);
    colorManager->addColor(COLOR_MAGENTA, COLOR_BLACK);
    colorManager->addColor(COLOR_GREEN, COLOR_BLACK);
    colorManager->addColor(COLOR_YELLOW, COLOR_BLACK);
    initHighlighter(colorManager);
}


void initHighlighter(ColorManager *colorManager){
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(#include)\\s+?"), COLOR_MAGENTA, COLOR_BLACK), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(#ifdef|#ifndef|#if|#define|#undef|#endif)"), COLOR_MAGENTA), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(#ifdef|#ifndef|#if|#define|#undef|#endif)\\s+?(.*?)[\\s$]"), COLOR_MAGENTA), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("#include\\s+?(<.*?>)"), COLOR_RED), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(\".*?\")"), COLOR_RED), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("[^\\w](\\d+?)[^\\w]"), COLOR_RED), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(true|false)"), COLOR_RED), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(this|return|private|public|protected|default)"), COLOR_YELLOW), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(int|const|void|bool|class)"), COLOR_GREEN), {"cc", "h"});
    colorManager->addHighlighter(std::make_shared<Highlighter>(std::regex("(//.*)|(/\\*[\\w\\W]*?\\*/)"), COLOR_BLUE), {"cc", "h"});
}
