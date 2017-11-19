#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <map>
class ColorManager{
private:
    std::map<int, std::map<int, int>> colors;
    size_t colorCounter;
public:
    ColorManager() = default;
    ~ColorManager() = default;
    void addColor(const int &fontColor, const int &bgColor);
    void init();
    void print(const std::string &str);
    void mvprint(const int &y, const int &x, const std::string &str);
    void printColor(const std::string &str, const int &fontColor, const int &bgColor);
    void printColor(const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor);
    void mvprintColor(const int &y, const int &x, const std::string &str, const int &fontColor, const int &bgColor);
};

#endif
