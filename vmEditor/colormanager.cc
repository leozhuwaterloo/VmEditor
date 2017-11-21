#include "colormanager.h"
#include <ncurses.h>
#include <sstream>
#include "highlighter.h"

void ColorManager::addColor(const int &fontColor, const int &bgColor){ colors[fontColor][bgColor] = (++colorCounter); }

void ColorManager::addHighlighter(std::shared_ptr<Highlighter> highlighter, std::vector<std::string> &&fileTypes){
    for(auto &fileType: fileTypes){
        highlighters[fileType].push_back(highlighter);
    }
}

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
    std::map<int, std::map<int, std::pair<int, int>>> highlight;
    for(auto &it: highlighters[fileType]){
        it->apply(_str, highlight);
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
