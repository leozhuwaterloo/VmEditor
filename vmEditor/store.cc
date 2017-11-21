#include "store.h"

Store::Store(std::list<std::string> strs):strs{strs}{}

std::string Store::getRenderString(const int &maxY) const {
    std::string res;

    int i = 0;
    std::list<std::string>::const_iterator it = strs.cbegin();
    for(;i < currY; ++i) ++it;
    i = 0;
    for(;it!= strs.cend(); ++it, ++i){
        if(i == maxY) break;
        res += (*it) + '\n';
    }
    return res;
}

std::list<std::string>& Store::getStrs() { return strs; }
void Store::moveY(const int &y){ currY += y; }
int Store::getCurrY() const { return currY; }
