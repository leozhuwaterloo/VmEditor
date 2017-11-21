#ifndef STORE_H
#define STORE_H
#include <string>
#include <list>

class Store{
private:
    std::list<std::string> strs;
    //int currY;
public:
    Store() = default;
    Store(std::list<std::string> strs);
    ~Store() = default;
    std::string getRenderString() const;
    std::list<std::string>& getStrs();
    //int getCurrY() const;
    //void moveY(const int &y);
};

#endif
