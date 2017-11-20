#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <regex>

class Highlighter{
private:
    std::regex reg;
    int fontColor;
    int bgColor;
public:
    Highlighter(std::regex reg, const int &fontColor, const int &bgColor);
    Highlighter(std::regex reg, const int &fontColor);
    void apply(std::string &str);
    virtual ~Highlighter() = default;

};

#endif
