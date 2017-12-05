#ifndef STORE_H
#define STORE_H
#include <string>
#include <list>

class Store{
private:
    std::list<std::string> strs;
    int currY, numInvalid, displayedLine;
    std::list<std::string>::const_iterator itCurrY;
public:
    Store(std::list<std::string> strs);
    Store(const Store &other);
    ~Store() = default;
    std::string getRenderString(const int &maxY, const int &maxX);
    void moveY(const int &y);
    std::list<std::string>& getStrs();
    std::list<std::string>::const_iterator& getItCurrY();
    int getCurrY() const;
    int getNumInvalid() const;
    int getDisplayedLine() const;
};

#endif
