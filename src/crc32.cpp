#include "../include/crc32.hpp"
#include "../include/bits.hpp"

vector<bool> computeCRC(vector<bool> data) {
    // Initialize CRC value
    vector<bool> crc;
    string generatorString = "11101101101110001000001100100000";
    vector<bool> generator;

    for (int i = 0; i < generatorString.length(); i++) {
        generator.push_back(generatorString[i] == '1');
    }

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

    for (int i = 0; i < generator.size() - 1; i++) {
        if (crc.size() == generator.size() - 1)
            break;
        else
            crc.insert(crc.begin() + i, false);
    }
    
    return crc;
}

bool decodeCRC(vector<bool>& data) {
    // Todo: Implement
    vector<bool> crc;
    string generatorString = "11101101101110001000001100100000";
    vector<bool> generator;

    for (int i = 0; i < generatorString.length(); i++) {
        generator.push_back(generatorString[i] == '1');
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

    for (int i = 0; i < crc.size(); i++) {
        if (crc[i] == true)
            return false;
    }

    for (int i = 0; i < generator.size() - 1; i++) {
        data.pop_back();
    }
    
    return true;
}