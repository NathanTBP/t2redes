#include "../include/bits.hpp"
#include <bitset>

vector<bool> stringToBinary(string s) {
    vector<bool> v;

    s=charStringToBinaryString(s);

    return v;
}

string charStringToBinaryString(string s) {

    string result;

    for (int i = 0; i < s.length(); i++) {
        result+=bitset<8>(int(s[i])).to_string();
    }

    return result;
}

string binaryToString(vector<bool> bits){

    int j;
    char letra;


}