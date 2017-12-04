#include "window.h"
#include "keylistener.h"
#include "command.h"
#include "event.h"
#include "colormanager.h"
#include "highlightergroup.h"
#include "highlighter.h"
#include "parser.h"
#include "saver.h"
#include <memory>
#include <ncurses.h>

void initCommands(KeyListener *keyListener);
void initColors(ColorManager *colorManagerr);
void initHighlighter(ColorManager *colorManagerr);


void initCommands(KeyListener *keyListener){
    keyListener->addCommand(std::make_unique<Commandi>());
    keyListener->addCommand(std::make_unique<CommandI>());
    keyListener->addCommand(std::make_unique<Commanda>());
    keyListener->addCommand(std::make_unique<CommandA>());
    keyListener->addCommand(std::make_unique<CommandUp>());
    keyListener->addCommand(std::make_unique<CommandDown>());
    keyListener->addCommand(std::make_unique<CommandLeft>());
    keyListener->addCommand(std::make_unique<CommandRight>());
    keyListener->addCommand(std::make_unique<Commandh>());
    keyListener->addCommand(std::make_unique<Commandl>());
    keyListener->addCommand(std::make_unique<Commandk>());
    keyListener->addCommand(std::make_unique<Commandj>());
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
    keyListener->addCommand(std::make_unique<CommandJ>());
    keyListener->addCommand(std::make_unique<CommandSlash>());
    keyListener->addCommand(std::make_unique<CommandQuestion>());
    keyListener->addCommand(std::make_unique<Commandn>());
    keyListener->addCommand(std::make_unique<CommandN>());
    keyListener->addCommand(std::make_unique<Commando>());
    keyListener->addCommand(std::make_unique<CommandO>());
    keyListener->addCommand(std::make_unique<CommandCtrlf>());
    keyListener->addCommand(std::make_unique<CommandCtrlg>());
    keyListener->addCommand(std::make_unique<CommandColon>());
    keyListener->addCommand(std::make_unique<Commandq>());
    keyListener->addCommand(std::make_unique<CommandAt>());
    keyListener->addCommand(std::make_unique<CommandDot>());
    keyListener->addCommand(std::make_unique<CommandSemi>());
    keyListener->addCommand(std::make_unique<CommandCtrld>());
    keyListener->addCommand(std::make_unique<CommandCtrlu>());
    keyListener->addCommand(std::make_unique<Commandc>());
    keyListener->addCommand(std::make_unique<Commandp>());
    keyListener->addCommand(std::make_unique<CommandP>());
}

void initColors(ColorManager *colorManager){
    colorManager->addColor(COLOR_WHITE, COLOR_BLACK);
    colorManager->addColor(COLOR_RED, COLOR_BLACK);
    colorManager->addColor(COLOR_BLUE, COLOR_BLACK);
    colorManager->addColor(COLOR_MAGENTA, COLOR_BLACK);
    colorManager->addColor(COLOR_GREEN, COLOR_BLACK);
    colorManager->addColor(COLOR_YELLOW, COLOR_BLACK);
    colorManager->addColor(COLOR_WHITE, COLOR_BLACK);
    colorManager->addColor(COLOR_CYAN, COLOR_BLACK);
    colorManager->addColor(COLOR_WHITE, COLOR_RED);
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
