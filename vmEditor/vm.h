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
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(//.*)|(/\\*.*?\\*/)"), COLOR_BLUE));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(#include)\\s+?"), COLOR_MAGENTA));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(#ifdef|#ifndef|#if|#define|#undef)\\s+?(.+)"), COLOR_MAGENTA));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("#include\\s+?(<.*?>)"), COLOR_RED));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(\".*?\")"), COLOR_RED));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("[^\\w](\\d+?)[^\\w]"), COLOR_RED));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(true|false)"), COLOR_RED));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(this|return)"), COLOR_YELLOW));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(int|const|void|bool)"), COLOR_GREEN));
    colorManager->addHighlighter("cc", std::make_unique<Highlighter>(std::regex("(//.*)|(/\\*[\\w\\W]*?\\*/)"), COLOR_BLUE));
}
