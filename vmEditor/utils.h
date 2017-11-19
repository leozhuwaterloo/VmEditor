void clamp(int &x, const int &low, const int &high){
    if(x < low){
        x = low;
    } else if (x >= high){
        x = high;
    }
}
