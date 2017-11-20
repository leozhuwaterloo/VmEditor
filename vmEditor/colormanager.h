#ifndef COLORMANAGER_H
#define COLORMANAGER_H
#include <map>
#include <vector>
#include <memory>
class Highlighter;

class ColorManager{
private:
    std::map<int, std::map<int, int>> colors;
    std::map<std::string, std::vector<std::unique_ptr<Highlighter>>> highlighters;
    std::string fileType;
    size_t colorCounter;
public:
    ColorManager() = default;
    ~ColorManager() = default;
    void addColor(const int &fontColor, const int &bgColor);
    void addHighlighter(const std::string &fileType, std::unique_ptr<Highlighter> highlighter);
    void init(const std::string &fileType);
    void print(const std::string &str);
    void mvprint(const int &y, const int &x, const std::string &str);
    void printColor(const std::string &str, const int &fontColor, const int &bgColor);
    void printColor(const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor, const int &bgColor);
};

#endif
