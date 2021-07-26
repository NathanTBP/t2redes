#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

/**
 * @brief Função para checar se a paridade da mensagem enviada corresponde com a paridade real da mensagem recebida.
 * 
 * @param bits mensagem que deve ser codificada
 * @param parity Se a paridade é par ou impar
 * @return true a paridade da mensagem é corresponde com a desejada
 * @return false a paridade da mensagem não corresponde com a desejada
 */
bool parityChecker(vector<bool>& bits, bool parity);
/**
 * @brief Função que adiciona o código binário de paridade no final da mensagem.
 * 
 * @param bits mensagem que deve ser codificada
 * @param parity Se a paridade é par ou impar
 * @return true a paridade da mensagem é corresponde com a desejada
 * @return false a paridade da mensagem não corresponde com a desejada
 */
bool addParity(vector<bool>& bits, bool parity);