#ifndef STORE_H
#define STORE_H
#include <string>
#include <list>

class Store{
private:
    std::list<std::string> strs;
    int currY, numInvalid;
    std::list<std::string>::const_iterator itCurrY;
public:
    Store() = default;
    Store(std::list<std::string> strs);
    ~Store() = default;
    std::string getRenderString(const int &maxY, const int &maxX);
    void moveY(const int &y);
    std::list<std::string>& getStrs();
    const std::list<std::string>::const_iterator& getItCurrY() const;
    int getCurrY() const;
    int getNumInvalid() const;
};

#endif
