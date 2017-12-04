#include "store.h"
#include <iostream>
Store::Store(std::list<std::string> strs):strs{strs}, currY{0}, numInvalid{0}{ itCurrY = getStrs().cbegin(); }

Store::Store(const Store &other): currY{other.getCurrY()}, numInvalid{other.getNumInvalid()}{
    strs.assign(other.strs.begin(), other.strs.end());
    itCurrY = strs.begin();
    for(int i = 0; i < currY; ++i) ++itCurrY;
};

std::string Store::getRenderString(const int &maxY, const int &maxX){
    std::string res;
    int i = 0;
    for(std::list<std::string>::const_iterator it = itCurrY; it!= strs.cend(); ++it){
        int lineLength = (*it).length() / maxX + 1;
        if(i + lineLength > maxY-1){ numInvalid = maxY - 1 - i; return res; }
        res += (*it) + "\n";
        i += lineLength;
    }
    numInvalid = 0;
    return res;
}

void Store::moveY(const int &y){
    currY += y;
    if(y > 0){
        for(int i = 0; i < y; ++i) ++itCurrY;
    } else if (y < 0){
        for(int i = 0; i > y; --i) --itCurrY;
    }
}

std::list<std::string>& Store::getStrs() { return strs; }
std::list<std::string>::const_iterator& Store::getItCurrY() { return itCurrY; }
int Store::getCurrY() const{ return currY; }
int Store::getNumInvalid() const { return numInvalid; }
