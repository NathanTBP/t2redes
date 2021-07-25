#include <stdio.h>       // Para entrada/saida (leitura e escrita via teclado monitor e arquivos)
#include <stdlib.h>      // Para alocação dinâmica
#include <string.h>      // Para manipular strings e setar memória
#include <time.h>        //necessário p/ função time()
#include <math.h>        // 
#include "../include/app.hpp"

using namespace std;

/*
    Relembrando as camadas da rede e suas principais funções:

    Aplicação - Interface entre usuário e rede
    Transporte - Conectar processos
    Rede - Conectar máquinas
    Enlace - Terminar o pacote e o enviar ao nó adjacente
    Física - Transmitir, em algum meio físico, os dados.

    No trabalho anterior, já foi tratado de portas e protocolos que englobam as camadas de transporte e rede, para esse trabalho,
    é considerado que isso já está feito e os dados vem de um usuário e passam direto a camada de enlace.
*/

// Aplicações: poderiam ser qualquer coisa (um jogo, um reprodutor de vídeo,etc) porém, no caso
// A aplicação transmissora transmite uma string inserida pelo usuário e a aplicação receptora imprime essa mensagem

int main (void) {
    App app = App();

    while (!app.canCloseMenu()) {
        app.loop();
    }

    return 0;
}