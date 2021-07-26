#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace std;

/**
 * @brief 
 * 
 */
class App {
    private:
        // Menu
        bool    isClosed = false;
        int     choosenOption = 0;

        // Message
        int     msgLength = 100;
        int     errorMethod = 2; // 0 = ?, 1 = ?, 2= ?
        int     tries = 5;
        float   errorProbability = 1.5;

        void welcomeMessage();
    public:
        App();

        bool canCloseMenu();
        void loop();

        /**
         * @brief Função que requisita do usuário uma mensagem, e a armazena na aplicação.
         * 
         * @return true mensagem enviada com sucesso.
         * @return false houve algum erro no envio da mensagem.
         */
        bool AplicacaoTransmissora();
        /**
         * @brief Função que transforma a mensagem do usuário em um vetor com valores binários.
         * Invoca uma função auxiliar para realizar esta operação.
         * 
         * @param msg mensagem enviada do usuário.
         * @return true mensagem enviada com sucesso.
         * @return false houve algum erro no envio da mensagem.
         */
        bool CamadaDeAplicacaoTransmissora(string msg);
        /**
         * @brief Envia a mensagem, já em bits, para receber o tratamento de erro antes de ser enviada.
         * 
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true mensagem enviada com sucesso.
         * @return false houve algum erro no envio da mensagem.
         */
        bool CamadaDeEnlaceDadosTransmissora(vector<bool> bits);
        /**
         * @brief checa, através do atributo "errorMethod", qual método de verificação de erros deve ser utilizado.
         * Em seguida, chama a respectiva função de tratamento de erro
         * 
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true mensagem enviada com sucesso.
         * @return false houve algum erro no envio da mensagem.
         */
        bool CamadaDeEnlaceDadosTransmissoraControleDeErro(vector<bool>& bits);
        bool MeioDeTransmissao(vector<bool> bits);
        /**
         * @brief Função que, dada a mensagem recebida, trata de verificar os erros na mensagem.
         * 
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true a mensagem foi recebida sem erros
         * @return false a mensagem recebida teve algum erro em sua transmissão.
         */
        bool CamadaDeEnlaceDadosReceptora(vector<bool> bits);
        /**
         * @brief checa, através do atributo "errorMethod", qual método de verificação de erros deve ser utilizado.
         * 
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true a mensagem foi recebida sem erros
         * @return false a mensagem recebida teve algum erro em sua transmissão.
         */
        bool CamadaDeEnlaceDadosReceptoraControleDeErro(vector<bool>& bits);
        /**
         * @brief Chama a Aplicação Receptora
         *          
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true a mensagem foi recebida sem erros
         * @return false a mensagem recebida teve algum erro em sua transmissão.
        */
        bool CamadaDeAplicacaoReceptora(vector<bool> bits);
        /**
         * @brief Função para imprimir a mensagem recebida, em binário, na tela.
         * 
         * @param bits mensagem do usuário transformara em um vetor de valores binários.
         * @return true a mensagem foi recebida sem erros
         * @return false a mensagem recebida teve algum erro em sua transmissão.
         */
        bool AplicacaoReceptora(vector<bool> bits);
};