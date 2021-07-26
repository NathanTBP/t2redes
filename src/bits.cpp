#include "../include/bits.hpp"

vector<bool> stringToBinary(string s) {
    vector<bool> v;

    s = charStringToBinaryString(s);

    for (int i = 0; i < s.length(); i++) {
        v.push_back(s[i] == '1');
    }

    return v;
}

string charStringToBinaryString(string s) {

    string result;

    for (int i = 0; i < s.length(); i++) {
        result += bitset<8>(int(s[i])).to_string();
    }

    return result;
}