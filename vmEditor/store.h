#ifndef STORE_H
#define STORE_H
#include <string>
#include <list>

class Store{
private:
    std::list<std::string> store;
public:
    Store() = default;
    Store(std::list<std::string> store);
    ~Store() = default;
    std::string getRenderString() const;
};

#endif
