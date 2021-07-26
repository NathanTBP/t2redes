#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

/**
 * @brief 
 * 
 * @param bits 
 * @return vector<bool> 
 */
vector<bool> computeCRC(vector<bool> bits);
/**
 * @brief 
 * 
 * @param bits 
 * @return true 
 * @return false 
 */
bool decodeCRC(vector<bool>& bits);