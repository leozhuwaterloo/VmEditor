#include "highlighter.h"
#include <ncurses.h>

Highlighter::Highlighter(std::regex reg, const int &fontColor, const int &bgColor):reg{reg},fontColor{fontColor}, bgColor{bgColor}{}
Highlighter::Highlighter(std::regex reg, const int &fontColor):Highlighter{reg, fontColor, COLOR_BLACK}{}


void Highlighter::apply(const std::string &str, std::map<int, std::map<int, std::pair<int, int>>> &highlight) const{
    std::string::const_iterator searchStart = str.cbegin();
    std::smatch m;
    while(std::regex_search(searchStart, str.cend(), m, reg)){
        for(unsigned i = 1; i < m.size(); ++i){
            highlight[searchStart - str.cbegin() + m.position(i)][m.length(i)] = std::make_pair(fontColor, bgColor);
        }
        searchStart += m.position() + m.length();
    }
}
