#include "colormanager.h"
#include <ncurses.h>
#include <sstream>
#include "highlightergroup.h"
#include "highlighter.h"

void ColorManager::addColor(const int &fontColor, const int &bgColor){ colors[fontColor][bgColor] = (++colorCounter); }

void ColorManager::addHighlighterGroup(std::unique_ptr<HighlighterGroup> highlighterGroup){ highlighterGroups.push_back(std::move(highlighterGroup));}

void ColorManager::init(const std::string &fileName){
    start_color();

    for(auto &it1 : colors){
        for(auto &it2 : it1.second){
            init_pair(it2.second, it1.first, it2.first);
        }
    }

    this->fileName = fileName;
}

void ColorManager::print(const std::string &str, const bool &highlight){
    if(highlight){
        std::string _str = str;
        std::map<int, std::map<int, std::pair<int, int>>> highlight;
        for(auto &it: highlighterGroups){
            if(it->match(fileName)) it->apply(_str, highlight);
        }

        std::stringstream ss{_str};
        ss >> std::noskipws;
        char c;
        int counter = 0;
        int fontColor, bgColor, to;
        bool colorMode = false;
        while(ss >> c){
            if(colorMode){
                --to;
                if(to == 0){
                    attroff(COLOR_PAIR(colors[fontColor][bgColor]));
                    colorMode = false;
                }
            }
            if(!colorMode && highlight.find(counter) != highlight.end()){
                 std::pair<int, std::pair<int, int>> pair = *(highlight[counter].begin());
                 to = pair.first;
                 fontColor = pair.second.first;
                 bgColor = pair.second.second;
                 attron(COLOR_PAIR(colors[fontColor][bgColor]));
                 colorMode = true;
            }
            ++counter;
            addch(c);
        }
        if(colorMode) attroff(COLOR_PAIR(colors[fontColor][bgColor]));
    } else {
        printw(str.c_str());
    }
}
void ColorManager::print(const std::string &str){ print(str, true); }
void ColorManager::mvprint(const int &y, const int &x, const std::string &str, const bool &highlight){ move(y, x); print(str, highlight); }
void ColorManager::mvprint(const int &y, const int &x, const std::string &str){ mvprint(y, x, str, true); }

void ColorManager::printColor(const std::string &str, const int &fontColor, const int &bgColor){
    attron(COLOR_PAIR(colors[fontColor][bgColor]));
    print(str, false);
    attroff(COLOR_PAIR(colors[fontColor][bgColor]));
}

void ColorManager::mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor, const int &bgColor){
    attron(COLOR_PAIR(colors[fontColor][bgColor]));
    mvprint(y, x, str, false);
    attroff(COLOR_PAIR(colors[fontColor][bgColor]));
}

void ColorManager::printColor(const std::string &str, const int &fontColor){ printColor(str, fontColor, COLOR_BLACK); }
void ColorManager::mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor){ mvprintColor(y, x, str, fontColor, COLOR_BLACK); }
