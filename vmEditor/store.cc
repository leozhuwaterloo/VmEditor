#include "store.h"

Store::Store(std::list<std::string> store):store{store}{}


std::string Store::getRenderString() const{
    std::string res;
    for(auto &it: store){
        res += it + '\n';
    }
    return res;
}
