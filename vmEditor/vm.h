#include "window.h"
#include "keylistener.h"
#include "command.h"
#include "event.h"
#include "colormanager.h"
#include "highlightergroup.h"
#include "highlighter.h"
#include "parser.h"
#include <memory>
#include <ncurses.h>

void initCommands(KeyListener *keyListener);
void initColors(ColorManager *colorManagerr);
void initHighlighter(ColorManager *colorManagerr);


void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<Commandi>());
    keyListener->addCommand(std::make_unique<Commanda>());
    keyListener->addCommand(std::make_unique<CommandUp>());
    keyListener->addCommand(std::make_unique<CommandDown>());
    keyListener->addCommand(std::make_unique<CommandLeft>());
    keyListener->addCommand(std::make_unique<CommandRight>());
    keyListener->addCommand(std::make_unique<CommandResize>());
    keyListener->addCommand(std::make_unique<Commandw>());
    keyListener->addCommand(std::make_unique<Commandb>());
    keyListener->addCommand(std::make_unique<Command0>());
    keyListener->addCommand(std::make_unique<CommandDollar>());
    keyListener->addCommand(std::make_unique<CommandCaret>());
    keyListener->addCommand(std::make_unique<Commandu>());
    keyListener->addCommand(std::make_unique<Commandx>());
    keyListener->addCommand(std::make_unique<CommandX>());
    keyListener->addCommand(std::make_unique<Commandf>());
    keyListener->addCommand(std::make_unique<CommandF>());
    keyListener->addCommand(std::make_unique<Commandr>());
    keyListener->addCommand(std::make_unique<CommandR>());
}

void initColors(ColorManager *colorManager){
    colorManager->addColor(COLOR_WHITE, COLOR_BLACK);
    colorManager->addColor(COLOR_RED, COLOR_BLACK);
    colorManager->addColor(COLOR_BLUE, COLOR_BLACK);
    colorManager->addColor(COLOR_MAGENTA, COLOR_BLACK);
    colorManager->addColor(COLOR_GREEN, COLOR_BLACK);
    colorManager->addColor(COLOR_YELLOW, COLOR_BLACK);
    colorManager->addColor(COLOR_WHITE, COLOR_BLACK);
    initHighlighter(colorManager);
}


void initHighlighter(ColorManager *colorManager){
    std::unique_ptr<HighlighterGroup> highlighterGroup = std::make_unique<HighlighterGroup>(std::initializer_list<std::regex>({
        std::regex(".*?\\.cc"), std::regex(".*?\\.h")}));


    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("(#include)\\s+?"), COLOR_MAGENTA));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("(#ifdef|#ifndef|#if|#define|#undef|#endif)"), COLOR_MAGENTA));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("(#ifdef|#ifndef|#if|#define|#undef|#endif)\\s+(\\S*?)[\\s$]"), COLOR_MAGENTA));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("#include\\s+(<.*?>)"), COLOR_RED));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("(\".*?\"|true|false)"), COLOR_RED));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("\\W(\\d+?)\\W"), COLOR_RED));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("\\W(this|return|private|public|protected|default)\\W"), COLOR_YELLOW));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("\\W(int|const|void|bool|class)\\W"), COLOR_GREEN));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("(//.*)|(/\\*[\\w\\W]*?\\*/)"), COLOR_BLUE));
    highlighterGroup->addHighlighter(std::make_unique<Highlighter>(std::regex("([^(/\\*)]*?\\*/)|(/\\*[^(\\*/)]*?)$"), COLOR_BLUE));

    colorManager->addHighlighterGroup(std::move(highlighterGroup));
}
