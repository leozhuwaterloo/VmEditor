#ifndef UTILS_H
#define UTILS_H

#include <string>
inline void clamp(int &x, const int &low, const int &high){
    if(x < low){
        x = low;
    } else if (x >= high){
        x = high;
    }
}


inline int clampReturn(const int &x, const int &low, const int &high){
    int _x = x;
    if(_x < low){
        _x = low;
    } else if (_x >= high){
        _x = high;
    }
    return _x;
}


inline std::string trim(const std::string &str) {
    size_t begin = str.find_first_not_of(" \t\n");
    if (begin == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n");
    return str.substr(begin, end - begin + 1);
}

inline std::string regexEscape(const std::string &str){
    std::string res;
    for(auto &it : str){
        res += "[" + std::string(1, it) + "]";
    }
    return res;
}


#endif
