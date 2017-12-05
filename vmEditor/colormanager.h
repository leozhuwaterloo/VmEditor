#ifndef COLORMANAGER_H
#define COLORMANAGER_H
#include <map>
#include <vector>
#include <memory>
#include <string>
class HighlighterGroup;
class Parser;

class ColorManager{
private:
    std::map<int, std::map<int, int>> colors;
    std::vector<std::unique_ptr<HighlighterGroup>> highlighterGroups;
    Parser *parser;
    size_t colorCounter;
public:
    ColorManager() = default;
    ~ColorManager() = default;
    void addColor(const int &fontColor, const int &bgColor);
    void addHighlighterGroup(std::unique_ptr<HighlighterGroup> highlighterGroup);
    void init(Parser *parser);
    void print(const std::string &str, const bool &highlight);
    void print(const std::string &str);
    void mvprint(const int &y, const int &x, const std::string &str, const bool &highlight);
    void mvprint(const int &y, const int &x, const std::string &str);
    void printColor(const std::string &str, const int &fontColor, const int &bgColor);
    void printColor(const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor, const int &bgColor);
};

#endif
