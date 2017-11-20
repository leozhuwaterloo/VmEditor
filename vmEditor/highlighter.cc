#include "highlighter.h"
#include <ncurses.h>

Highlighter::Highlighter(std::regex reg, const int &fontColor, const int &bgColor):reg{reg},fontColor{fontColor}, bgColor{bgColor}{}
Highlighter::Highlighter(std::regex reg, const int &fontColor):Highlighter{reg, fontColor, COLOR_BLACK}{}



void Highlighter::apply(std::string &str){
    str = std::regex_replace (str, reg, "\201\201"+std::to_string(fontColor)+"\201"+std::to_string(bgColor)+"$1"+"\201");
}
