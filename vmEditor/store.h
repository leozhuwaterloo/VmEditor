#ifndef STORE_H
#define STORE_H
#include <string>
#include <vector>
#include <map>

class Store{
private:
    std::map<int, std::vector<std::string>> strs;
    size_t currY;
public:
    Store() = default;
    Store(std::map<int, std::vector<std::string>> strs);
    ~Store() = default;
    std::string getRenderString() const;
    int getLineLength(const int &line) const;
};

#endif
