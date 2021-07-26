#include <iostream>
#include <stdlib.h>
#include <vector>
#include <bitset>

using namespace std;

/**
 * @brief Função para converter uma string em um vetor de valores binários
 * 
 * @param s string que quer ser convertida
 * @return vector<bool> vetor que representa a string s em binário
 */
vector<bool> stringToBinary(string s);

/**
 * @brief Função que converte uma string para valores binários.
 * Contudo, nesta função, os valores binários estão armazenados em uma string.
 * 
 * @param s string que quer ser convertida
 * @return string string que representa a string s em binário
 */
string charStringToBinaryString(string s);