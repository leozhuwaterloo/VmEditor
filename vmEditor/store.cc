#include "store.h"

Store::Store(std::map<int, std::vector<std::string>> strs):strs{strs}{}

std::string Store::getRenderString() const {
    std::string res;
    for(auto &it1: strs){
        for(auto &it2: it1.second){
            res += it2 + '\n';
        }
    }
    return res;
}

int Store::getLineLength(const int &line) const {
    int res = strs.at(line+currY).at(0).length();
    if(res == 0) res = 1;
    return res;
}
