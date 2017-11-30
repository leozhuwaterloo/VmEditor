#ifndef UTILS_H
#define UTILS_H

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

#endif
