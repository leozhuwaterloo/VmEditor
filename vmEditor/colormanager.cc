#include "colormanager.h"
#include <ncurses.h>
#include <string>

void ColorManager::addColor(const int &fontColor, const int &bgColor){
    colors[fontColor][bgColor] = (++colorCounter);
}


void ColorManager::init(){
    start_color();

    for(auto &it1 : colors){
        for(auto &it2 : it1.second){
            init_pair(it2.second, it1.first, it2.first);
        }
    }
}

void ColorManager::print(const std::string &str){ printw(str.c_str()); }
void ColorManager::mvprint(const int &y, const int &x, const std::string &str){ mvprintw(y, x, str.c_str()); }

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
