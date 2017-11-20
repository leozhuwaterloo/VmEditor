#include "colormanager.h"
#include <ncurses.h>
#include <sstream>
#include "highlighter.h"

void ColorManager::addColor(const int &fontColor, const int &bgColor){ colors[fontColor][bgColor] = (++colorCounter); }

void ColorManager::addHighlighter(const std::string &fileType, std::unique_ptr<Highlighter> highlighter){ highlighters[fileType].push_back(std::move(highlighter)); }

void ColorManager::init(const std::string &fileType){
    start_color();

    for(auto &it1 : colors){
        for(auto &it2 : it1.second){
            init_pair(it2.second, it1.first, it2.first);
        }
    }

    this->fileType = fileType;
}

void ColorManager::print(const std::string &str){
    std::string _str = str;
    for(auto &it: highlighters[fileType]){
        it->apply(_str);
    }

    std::stringstream ss{_str};
    ss >> std::noskipws;
    char c, preC;
    bool colorMode = false;
    int fontColor, bgColor;
    while(ss >> c){
        if(c == '\201'){
            if(preC == '\201'){
                ss >> fontColor;
                ss >> c >> bgColor;
                attron(COLOR_PAIR(colors[fontColor][bgColor]));
                colorMode = true;
            } else if(colorMode){
                attroff(COLOR_PAIR(colors[fontColor][bgColor]));
                colorMode = false;
            }
        }else{
            addch(c);
        }
        preC = c;
    }
}

void ColorManager::mvprint(const int &y, const int &x, const std::string &str){ move(y, x); print(str); }

void ColorManager::printColor(const std::string &str, const int &fontColor, const int &bgColor){
    attron(COLOR_PAIR(colors[fontColor][bgColor]));
    print(str);
    attroff(COLOR_PAIR(colors[fontColor][bgColor]));
}

void ColorManager::mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor, const int &bgColor){
    attron(COLOR_PAIR(colors[fontColor][bgColor]));
    mvprint(y, x, str);
    attroff(COLOR_PAIR(colors[fontColor][bgColor]));
}

void ColorManager::printColor(const std::string &str, const int &fontColor){ printColor(str, fontColor, COLOR_BLACK); }
void ColorManager::mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor){ mvprintColor(y, x, str, fontColor, COLOR_BLACK); }
