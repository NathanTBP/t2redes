#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

/**
 * @brief Função que aplica o método CRC para detecção de erro na mensagem enviada.
 * Adiciona os bits que codificam a mensagem antes de ser enviada.
 * 
 * @param bits mensagem que deve ser codificada.
 * @return vector<bool> mensagem já codificada com CRC.
 */
vector<bool> computeCRC(vector<bool> bits);
/**
 * @brief Decodifica uma mensagem com CRC e verifica se existem erros.
 * 
 * @param bits mensagem recebida, que deve ser decodificada.
 * @return true a mensagem está correta.
 * @return false existem erros na mensagem.
 */
bool decodeCRC(vector<bool>& bits);