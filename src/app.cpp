#include "../include/app.hpp"
#include "../include/bits.hpp"
#include <iostream>
#include <time.h>

App::App() {
    this->welcomeMessage();
}

void App::welcomeMessage() {
    cout << "Ola! Bem vindo(a) ao simulador de transmissao de dados v1.0.1" << endl;
    cout << "1: Iniciar Programa!" << endl;
    cout << "2: Metodo de controle de erros [1=CRC,2=Bit Impar,3=Bit Par,4=Enquadramento] (Atual = " << this->errorMethod << ")" << endl;
    cout << "3: Porcentagem de chance de ocorrer um erro em cada bit da transmissao (Atual = " << this->errorProbability << ")" << endl;
    cout << "4: Tamanho maximo da mensagem (Atual = " << this->msgLength << " caracteres)" << endl;
    cout << "5: Tentativas de reenviar a mensagem com erro (Atual = " << this->tries << " tentativas)" << endl;
    cout << "6: Mostrar o menu" << endl;
    cout << "7: Sair do Programa :(" << endl;
}

bool App::canCloseMenu() {
    return !this->isClosed;
}

void App::loop() {
    cout << "Por favor, utilize a tecla correspondente do menu para alterar as variaveis." << endl;
    cin >> this->choosenOption;

    switch (this->choosenOption) {
        case 1:
            if (AplicacaoTransmissora())
                cout << "Mensagem enviada com sucesso!" << endl;
            else
                cout << "Ocorreu um erro e a mensagem nao pode ser enviada!" << endl;
            
            break;
        case 2:
            do {
                cout << "Escolha o metodo de controle de erros desejado:" << endl << "1=CRC" << endl << "2= Bit de paridade Impar" << endl << "3= Bit de paridade Par" << endl << "4= Algoritmo de enquadramento" << endl;
                cin >> this->errorMethod;
            } while (!(this->errorMethod > 0 && this->errorMethod < 5));

            break;
        case 3:
            do {
                cout << "Escolha a porcentagem de chance de ocorrer um erro a cada bit da transmissao:" << endl;
                cin >> this->errorProbability;
            } while (!(this->errorProbability > 0 && this->errorProbability < 100));

            break;
        case 4:
            do {
                cout << "Escolha o tamanho maximo de cada mensagem da transmissao (até 65536):" << endl;
                cin >> this->msgLength;
            } while (!(this->msgLength < 65536)); // 16 BITS
            
            break;
        case 5:
            cout << "Escolha o numero de tentativas que a camada de enlace vai tentar reenviar a mensagem até retornar erro." << endl;
            cin >> this->tries;

            break;
        case 6:
            this->welcomeMessage();

            break;
        case 7:            
            this->isClosed = true;

            break;
        default:
            cout << "Escolha nao valida! Tente Novamente." << endl;

            break;
    }
}

bool App::AplicacaoTransmissora() {
    // A aplicação transmissora quer, requisitar uma string do usuário e enviá-la a outra máquina
    string msg;

    cout << "Digite uma mensagem (tamanho requirido:" << this->msgLength <<  "):" << endl;

    while (msg.length() > this->msgLength)
        cin >> msg;
    
    // Chama a primeira camada (Aplicação)
    if (this->CamadaDeAplicacaoTransmissora(msg))
        return true;
    
    return false;
}

bool App::CamadaDeAplicacaoTransmissora(string msg) {
    // A camada de Aplicação transmissora quer, receber uma string da aplicação, transformá-la em bits e enviá-la a próxima camada
    // Que usualmente é a camada de Transporte, porém, neste trabalho, é diretamente a de enlace

    vector<bool> bits;
    bits=stringToBinary(msg);

    if (this->CamadaDeEnlaceDadosTransmissora(bits))
        return true;
    
    return false;
}

bool App::CamadaDeEnlaceDadosTransmissora(vector<bool> bits) {
    // Chama o controle de Erros

    if (this->CamadaDeEnlaceDadosTransmissoraControleDeErro(bits)) {
        // Faz o número de tentativas necessárias a função que envia os dados, se nenhuma der certo, retorna um erro
        for (int i = 0; i < this->tries; i++) {
            // Se a mensagem foi enviada com sucesso, retorna sucesso
            if (this->MeioDeTransmissao(bits))
                return true;
        }
    }

    return false;
}

bool App::CamadaDeEnlaceDadosTransmissoraControleDeErro(vector<bool> &bits) {
    switch (this->errorMethod) {
        case 1: // CRC32            
            break;
        case 2: // Paridade Par
        bool paridade=true; // Ao iniciar, o número de '1's é 0, logo a paridade é verdadeira
        for(int i=0;i<bits.size();i++) if(bits[i]) paridade=!paridade;
        bits.push_back(paridade);
            break;
        case 3: // Paridade Impar
        bool paridade=false; // Ao iniciar, o número de '1's é 1, logo a paridade é falsa
        for(int i=0;i<bits.size();i++) if(bits[i]) paridade=!paridade;
        bits.push_back(paridade);
            break;
        default:
            break;
    }

    return true;
}

bool App::MeioDeTransmissao(vector<bool> bits) {

    int i;
    float random;

    vector<bool> fluxA; // Bits que saem do computador A
    vector<bool> fluxB(bits.size()); // Bits que chegam ao computador B

    // Seta a semente aleatória (para ocorrerem erros de flip de bits)
    srand(time(NULL));

    // O fluxo que sai de A é a mensagem enviada para o Meio (é necessária uma cópia, para não perder os dados originais, caso haja uma mudança no meio)
    // Lembrando que o simulador não tem como foco a performance mas sim, a simulação.
    fluxA = bits;

    //Começa a transmitir os dados (8 é o numero de bits de cada caracter e 16 é o numero de bits que representam o tamanho)
    for(i=0;i<bits.size();i++){
        //O bit vai sendo normalmente transferido
        fluxB[i]=fluxA[i];
        //E aqui a probabilidade de um bit ser 'flipado' entra:
        //Se um numero aleatorio, com 2 casas decimais entre 1 e 100 for menor ou igual que a probabilidade de dar erro, o erro acontece e o bit é flipado
        random=((float)(rand()%100+1)+((float)(rand()%100))/100);
        if(random<=this->errorProbability) fluxB[i]=!fluxB[i];
    }

    //Passa os dados para camada de enlace receptora, se ela não detectou erros, ela envia um ack que retorna para a camada transmissora confirmando o envio correto dos dados
    //Se os dados não estavam íntegros na camada de enlace receptora, um sinal é enviado para a camada transmissora, pedindo a retransmissão.
    return CamadaDeEnlaceDadosReceptora(fluxB);
    
}

bool App::CamadaDeEnlaceDadosReceptora(vector<bool> bits) {
    // Se a mensagem está correta
    if (this->CamadaDeEnlaceDadosReceptoraControleDeErro(bits)) {
        // Manda a mensagem para a aplicação
        this->CamadaDeAplicacaoReceptora(bits);

        // Manda um sinal de deu certo para o meio
        return true;
    }

    return false;
}

bool App::CamadaDeEnlaceDadosReceptoraControleDeErro(vector<bool> bits) {
    switch (this->errorMethod) {
        case 1: // CRC32            
            break;
        case 2: // Paridade Par
        bool paridade=true;
        for(int i=0;i<bits.size();i++) if(bits[i]) paridade=!paridade;
        if(paridade==bits.back()){
            bits.pop_back();
            return true;
        }
        else return false;
            break;
        case 3: // Paridade Impar
        bool paridade=false; // Ao iniciar, o número de '1's é 1, logo a paridade é falsa
        for(int i=0;i<bits.size();i++) if(bits[i]) paridade=!paridade;
        if(paridade==bits.back()){
            bits.pop_back();
            return true;
        }
        else return false;
        
            break;
        default:
            break;
    }

    return true;
}

bool App::CamadaDeAplicacaoReceptora(vector<bool> bits) {
    // A camada de apicação receptura, recebe uma array de bits

    string mensagem;

    mensagem = binaryToString(bits);

    if (this->AplicacaoReceptora(mensagem))
        return true;

    // Ocorreu algum erro
    return false;
}

bool App::AplicacaoReceptora(string mensagem) {
    // A aplicação receptora quer imprimir a mensagem na tela

    cout << "A mensagem recebida foi: " << mensagem << endl;

    return true;
}