#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include <regex>
#include <vector>
#include <initializer_list>
#include <string>
#include <map>

class Highlighter{
private:
    std::regex reg;
    int fontColor;
    int bgColor;
public:
    Highlighter(const std::regex &reg, const int &fontColor, const int &bgColor);
    Highlighter(const std::regex &reg, const int &fontColor);
    ~Highlighter() = default;
    void apply(const std::string &str, std::map<int, std::map<int, std::pair<int, int>>> &highlight) const;
};

#endif
