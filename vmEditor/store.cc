#include "store.h"

Store::Store(std::list<std::string> strs):strs{strs}{ itCurrY = getStrs().cbegin(); }


std::string Store::getRenderString(const int &maxY, const int &maxX){
    std::string res;
    int i = 0;
    for(std::list<std::string>::const_iterator it = itCurrY; it!= strs.cend(); ++it){
        int lineLength = (*it).length() / maxX + 1;
        if(i + lineLength > maxY-1) break;
        res += (*it) + "\n";
        i += lineLength;
    }
    numInvalid = maxY - 1 - i;
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
