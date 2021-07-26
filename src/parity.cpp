#include "../include/parity.hpp"

bool parityChecker(vector<bool>& bits, bool parity) {
    for (int i = 0; i < bits.size(); i++) {
        if (bits[i])
            parity = !parity;
    }

    if (parity == bits.back()) {
        bits.pop_back();
        
        return true;
    }

    return false;
}

bool addParity(vector<bool>& bits, bool parity) {
    // Ao iniciar, o número de '1's é 0, logo a paridade é verdadeira
    for (int i = 0; i < bits.size(); i++) {
        if(bits[i])
            parity = !parity;
    }

    bits.push_back(parity);

    return true;
}