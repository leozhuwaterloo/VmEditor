#include "store.h"

Store::Store(std::list<std::string> strs):strs{strs}{}

std::string Store::getRenderString() const {
    std::string res;
    for(auto &it: strs){
        res += it + '\n';
    }
    return res;
}


std::list<std::string>& Store::getStrs() { return strs; }
