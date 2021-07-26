#include "../include/crc32.hpp"
#include "../include/bits.hpp"

vector<bool> computeCRC(vector<bool> data) {
    // Initialize CRC value
    vector<bool> crc;
    vector<bool> generator = stringToBinary("11011");

    for (int i = 0; i < generator.size() - 1; i++) {
        data.push_back(false);
    }
    
    for (int i = 0; i < data.size(); i++) {
        if (crc.size() == 0) {
            if (data[i] == true)
                crc.push_back(true);
            else
                continue;
        } else {
            crc.push_back(data[i]);

            if (crc.size() == generator.size()) {
                for (int j = 0; j < generator.size(); j++) {
                    if (crc[j] == true) {
                        crc[j] = (generator[j] == true) ? false : true;
                    } else {
                        crc[j] = (generator[j] == true) ? true : false;
                    }
                }

                // Check for leading zeros
                for (int j = 0; j < crc.size(); j++) {
                    if (crc[j] == true)
                        break;
                    else {
                        crc.erase(crc.begin() + j);
                        j--;
                    }
                }
            }
        }
    }

    for (int i = 0; i < generator.size(); i++) {
        if (crc.size() == generator.size())
            break;
        else
            crc.insert(crc.begin() + i, false);
    }
    
    return crc;
}

bool decodeCRC(vector<bool> bits) {
    // Todo: Implement
    vector<bool> crc;
    vector<bool> generator = stringToBinary("11011");
    
    return true;
}