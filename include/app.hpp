#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

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

        //
        bool AplicacaoTransmissora();
        bool CamadaDeAplicacaoTransmissora(string msg);
        bool CamadaDeEnlaceDadosTransmissora(vector<bool> bits);
        bool CamadaDeEnlaceDadosTransmissoraControleDeErro(vector<bool>& bits);
        bool MeioDeTransmissao(vector<bool> bits);
        bool CamadaDeEnlaceDadosReceptora(vector<bool> bits);
        bool CamadaDeEnlaceDadosReceptoraControleDeErro(vector<bool> bits);
        bool CamadaDeAplicacaoReceptora(vector<bool> bits);
        bool AplicacaoReceptora(string mensagem);
};