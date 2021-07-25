#include "../include/bits.hpp"

vector<bool> stringToBinary(string s) {
    vector<bool> v;

    for (int i = 0; i < s.length(); i++) {
        v.push_back(s[i] == '1');
    }

    return v;
}