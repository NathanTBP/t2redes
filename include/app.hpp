#include <iostream>
#include <stdlib.h>
#include <vector>

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
        int     errorMethod = 0; // 0 = ?, 1 = ?, 2= ?
        int     errorProbability = 0;
        int     tries = 0;

        void welcomeMessage();
    public:
        App();

        bool canCloseMenu();
        void loop();

        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        bool AplicacaoTransmissora();
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeAplicacaoTransmissora(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeEnlaceDadosTransmissora(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeEnlaceDadosTransmissoraControleDeErro(vector<bool> bits);
        bool MeioDeTransmissao(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeEnlaceDadosReceptora(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeEnlaceDadosReceptoraControleDeErro(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool CamadaDeAplicacaoReceptora(vector<bool> bits);
        /**
         * @brief 
         * 
         * @param bits 
         * @return true 
         * @return false 
         */
        bool AplicacaoReceptora(vector<bool> bits);
};