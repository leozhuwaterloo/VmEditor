#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <regex>
#include <map>
class Highlighter{
private:
    std::regex reg;
    int fontColor;
    int bgColor;
public:
    Highlighter(std::regex reg, const int &fontColor, const int &bgColor);
    Highlighter(std::regex reg, const int &fontColor);
    void apply(const std::string &str, std::map<int, std::map<int, std::pair<int, int>>> &highlight) const;
    virtual ~Highlighter() = default;

};

#endif
