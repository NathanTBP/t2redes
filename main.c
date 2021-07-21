/* Objetivo: Fazer um programa que simule o envio um dado de um computador 'A' para um computador computador 'B' 
realizando as devidas conversões entre String, da mensagem a ser enviada, e os bytes e bits a serem
verificados pela camada de enlace, passando por um meio de transporte possivelmente ruidoso, que pode 'flipar' os bits
então, métodos de verificação de erros também são implementados.
*/
#include <stdio.h>       // Para entrada/saida (leitura e escrita via teclado monitor e arquivos)
#include <stdlib.h>      // Para alocação dinâmica
#include <string.h>      // Para manipular strings e setar memória
#include <time.h>        //necessário p/ função time()

//Definição de um tipo booleano para C -> Unsigned char é a menor unidade de dados possível na linguagem.
//Será usado, tanto no retorno de funções (deu certo ou não) quanto para representar os bits sendo 0 FALSE e 1 TRUE
#define boolean unsigned char 
#define TRUE 1
#define FALSE 0

struct vetorb{
    boolean* vetor;
    int tamanho;
};

typedef struct vetorb Vetorb;

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

//Aplicações: poderiam ser qualquer coisa (um jogo, um reprodutor de vídeo,etc) porém, no caso
// A aplicação transmissora transmite uma string inserida pelo usuário e a aplicação receptora imprime essa mensagem
boolean AplicacaoTransmissora(void);
boolean AplicacaoReceptora(char* mensagem);

//Camada De aplicação: funciona como o SO do sistema, integrado com algumas camadas extras como transporte e rede tem como
//objetivo decodificar a mensagens de alto nível (strings ou inteiros) de/para bits e envia-las para a rede 
// (no caso as camadas de Enlace e a aplicação)
boolean CamadaDeAplicacaoTransmissora(char* mensagem);
boolean CamadaDeAplicacaoReceptora(boolean* msg);
//Funções de apoio para a camada de aplicação: Realizam conversões de strings e inteiros para bits e vice-versa
boolean* StringToBits(char* string);
boolean* IntTo16Bits(int i);
char* BitsToString(boolean* msg16bits);
int Bits16ToInt(boolean* int16bits);
int Bits8ToInt(boolean* int8bits);
boolean* copiaVetor(boolean* vetorcopiado, int tamanho);

//Camada de enlace: atua como ultimo mecanismo antes de enviar/receber os dados. Ela procura por erros e prepara o pacote
// Para enviá-lo ao meio de transporte 
boolean CamadaDeEnlaceDadosTransmissora(boolean* msg);
Vetorb CamadaDeEnlaceDadosTransmissoraControleDeErro(boolean* msg);
boolean CamadaDeEnlaceDadosReceptora(boolean* msg);
boolean* CamadaDeEnlaceDadosReceptoraControleDeErro(boolean* msg);

//Um meio de transmissao ruidoso, que pode 'flipar' os bits.
boolean MeioDeTransmissao(boolean* msg, int tamanhototal);
boolean Flip(boolean bit); // Flipa um bit


//Variáveis globais referentes a simulação
unsigned int METODO_DE_ERROS = 2; //Qual método de controle erros será usado
float CHANCE_DE_ERRO = 1.5; // Qual a chance de ocorrer um erro durante a transmissão
unsigned int TAMANHO_MAXIMO_DA_MENSAGEM = 100; // O tamanho máximo da mensagem a ser enviada
unsigned int TENTATIVAS_REENVIO = 5; // Quantas vezes a camada de enlace tenta reenviar os dados.

//O programa principal faz a interface com o usuário, para que ele defina as variáveis envolvidas na transmissão da mensagem
int main (void){

    int variaveldomenu;
    boolean loop=TRUE;

    printf("Ola! Bem vindo(a) ao simulador de transmissao de dados v1.0.1\n");
    printf("1: Iniciar Programa!\n");
    printf("2: Metodo de controle de erros [1=CRC,2=Bit Impar,3=Bit Par,4=Enquadramento] (Atual = %d)\n",METODO_DE_ERROS);
    printf("3: Porcentagem de chance de ocorrer um erro em cada bit da transmissao (Atual = %f%%)\n",CHANCE_DE_ERRO);
    printf("4: Tamanho maximo da mensagem (Atual = %d caracteres)\n",TAMANHO_MAXIMO_DA_MENSAGEM);
    printf("5: Tentativas de reenviar a mensagem com erro (Atual = %d tentativas)\n",TENTATIVAS_REENVIO);
    printf("6: Mostrar o menu\n");
    printf("7: Sair do Programa :(\n");

    do{

    printf("Por favor, utilize a tecla correspondente do menu para alterar as variaveis\n");

    scanf(" %d",&variaveldomenu);

    switch (variaveldomenu)
    {
    case 1:
         fflush(stdin);
         if(AplicacaoTransmissora()) printf("Mensagem enviada com sucesso!\n");
         else printf("Ocorreu um erro e a mensagem nao pode ser enviada\n");
        break;
    case 2:
        do{
        printf("Escolha o metodo de controle de erros desejado:\n1=CRC\n2= Bit de paridade Impar\n3= Bit de paridade Par\n4= Algoritmo de enquadramento\n");
        scanf(" %d",&METODO_DE_ERROS);
        }while(!(METODO_DE_ERROS>0 && METODO_DE_ERROS<5));
        break;
    case 3:
        do{
        printf("Escolha a porcentagem de chance de ocorrer um erro a cada bit da transmissao:\n");
        scanf(" %f",&CHANCE_DE_ERRO);
        }while(!(CHANCE_DE_ERRO>0 && CHANCE_DE_ERRO<100));
        break;
    case 4:
        do{
        printf("Escolha o tamanho maximo de cada mensagem da transmissao (até 65536):\n");
        scanf(" %d",&TAMANHO_MAXIMO_DA_MENSAGEM);
        }while(!(TAMANHO_MAXIMO_DA_MENSAGEM<65536)); // 16 BITS
        
        break;
    case 5:
        printf("Escolha o numero de tentativas que a camada de enlace vai tentar reenviar a mensagem até retornar erro\n");
        scanf(" %d",&TENTATIVAS_REENVIO);
        break;
    case 6:
        printf("1: Iniciar Programa!\n");
        printf("2: Metodo de controle de erros [1=CRC,2=Bit Impar,3=Bit Par,4=Enquadramento] (Atual = %d)\n",METODO_DE_ERROS);
        printf("3: Porcentagem de chance de ocorrer um erro em cada bit da transmissao (Atual = %f%%)\n",CHANCE_DE_ERRO);
        printf("4: Tamanho maximo da mensagem (Atual = %d caracteres)\n",TAMANHO_MAXIMO_DA_MENSAGEM);
        printf("5: Tentativas de reenviar a mensagem com erro (Atual = %d tentativas)\n",TENTATIVAS_REENVIO);
        printf("6: Mostrar o menu\n");
        printf("7: Sair do Programa :(\n");
        break;
    case 7:
        loop=FALSE; // quebra o laço do menu e permite que o programa saia em segurança
        break;
    default:
        printf("Escolha nao valida! Tente Novamente \n");
        break;
    }
    }while(loop==TRUE);

    return 0;
    
}

boolean AplicacaoTransmissora(void){

    //A aplicação transmissora quer, requisitar uma string do usuário e enviá-la a outra máquina

    char* mensagem;
    boolean deucerto;

    mensagem=(char*)malloc(sizeof(char)*TAMANHO_MAXIMO_DA_MENSAGEM);
    if(mensagem==NULL) return FALSE; // Se a mensagem não pode ser alocada, é emitido um erro para a transmissão

    printf("Digite uma mensagem:\n");
    fgets(mensagem,TAMANHO_MAXIMO_DA_MENSAGEM,stdin); // A mensagem é um conjunto de caracteres, de tamanho até o máximo + o \n + o \0 no final

    if(CamadaDeAplicacaoTransmissora(mensagem)) deucerto= TRUE; // Chama a primeira camada (Aplicação)

    // Libera a mensagem (a aplicação não faz mais nada com ela)
    free(mensagem);

    if(deucerto==TRUE) return TRUE;
    else return FALSE;

}

boolean AplicacaoReceptora(char* mensagem){

    //A aplicação receptora quer, receber uma string e imprimi-la na tela.

    if(mensagem!=NULL) { // Se a mensagem é uma string de fato, a imprime
    printf("A mensagem recebida foi: ");
    fputs(mensagem,stdout);

    //A Aplicação não vai usar mais a mensagem:
    free(mensagem);

    return TRUE; // Deu tudo certo
    }

    return FALSE; // Retorna erro

}

boolean CamadaDeAplicacaoTransmissora(char* mensagem){

    //A camada de Aplicação transmissora quer, receber uma string da aplicação, transformá-la em bits e enviá-la a próxima camada
    //Que usualmente é a camada de Transporte, porém, neste trabalho, é diretamente a de enlace

    boolean* msg; // Como são bits, os valores válidos são 0 e 1 (true ou falso)
    boolean deucerto=FALSE;

    //Função que converte uma string para um grande array de bits
    //Os 16 primeiros bits representam o número de caracteres da string em binário
    msg=StringToBits(mensagem);

    if(msg!=NULL){
        
        //Nesse momento, são considerados que os endereços estão certos, apenas o dado precisa ser enviado
        // Logo, a esse "pulo" direto entre as camadas de aplicação e enlace
        if(CamadaDeEnlaceDadosTransmissora(msg)) deucerto=TRUE;
    
    }
    //Libera a mensagem (binária)
    free(msg);

    //Retorna se funcionou ou não
    if(deucerto==TRUE) return TRUE;
    else return FALSE; 

}

boolean CamadaDeAplicacaoReceptora(boolean* msg){

    //A camada de apicação receptura, recebe uma array de bits e o transforma em uma mensagem em string

    char* mensagem;

    //Função que converte um grande array de bits para uma string (os 16 primeiros bytes são o tamanho)
    mensagem=BitsToString(msg);

    if(mensagem!=NULL){ // Se a mensagem foi recebida corretamente
        
        if(AplicacaoReceptora(mensagem)==TRUE){

            return TRUE; // Avisa que deu tudo certo
        } // Passa a mensagem para a aplicação
       
    
    }

    return FALSE; // Ocorreu algum erro

}

boolean CamadaDeEnlaceDadosTransmissora(boolean* msg){

    int tentativas;

    Vetorb mensagemfiltrada;

    //Chama o controle de Erros
    mensagemfiltrada=CamadaDeEnlaceDadosTransmissoraControleDeErro(msg);
    if(mensagemfiltrada.tamanho!=-1){ // Se o erro foi filtrado, pode chamar a camada física (meio de transmissão)
        
        //Para enviar corretamente a mensagem, é necessário conhecer o tamanho total dela, para evitar verificações desse tipo no meio (o meio não verifica nada)


        for(tentativas=TENTATIVAS_REENVIO;tentativas>0;tentativas--){ // Faz o número de tentativas necessárias a função que envia os dados, se nenhuma der certo, retorna um erro
            if(MeioDeTransmissao(mensagemfiltrada.vetor,mensagemfiltrada.tamanho)==TRUE) return TRUE; // Se a mensagem foi enviada com sucesso, retorna sucesso
        }
        
    } 

        return FALSE; // Ocorreu um erro na transmissão (numero de chamadas excedido )

}


Vetorb CamadaDeEnlaceDadosTransmissoraControleDeErro(boolean* msg){

    int i,tamanho;
    boolean paridade;
    //Como, para o método de controle de erros são necessários adicionar/remover bits

    boolean* msgcc; // Representa a mensagem SEM controle
    Vetorb resultado;

    tamanho=Bits16ToInt(msg);// Captura o tamanho da mensagem

    switch (METODO_DE_ERROS)
    {
    case 1: // CRC
        //Sera usado o CRC 32, ou seja, o polinomio divisor tem 33 bits
        //O polinomio usado é x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1 ou 10000010 01100000 10001110 11011011 1



        break;
    case 2: // Paridade Par
        // Para a verificação da partidade par, é necessário um bit ao final da mensagem, que conta se o numero de '1's é par 
        msgcc=(boolean*)malloc(sizeof(boolean)*(tamanho*8)+17); // 8 bits por letra + 16 que representa o tamanho no inicio + o bit de paridade no fim
        if(msgcc==NULL) {
            tamanho=-1;
            return resultado;
        }
        paridade=TRUE; // Ao iniciar, o número de '1's é 0, logo a paridade é verdadeira
        for(i=0;i<(tamanho*8)+16;i++){
            msgcc[i]=msg[i]; // Copia o valor do vetor original
            if(msg[i]==TRUE) paridade=Flip(paridade); // Ajusta a partidade
        }
        msgcc[(tamanho*8)+16]=paridade; // Coloca o bit de paridade no final
        resultado.tamanho=(tamanho*8)+17;
        break;
    case 3: // Paridade Impar
        // Para a verificação da partidade par, é necessário um bit ao final da mensagem, que conta se o numero de '1's é impar
        msgcc=(boolean*)malloc(sizeof(boolean)*(tamanho*8)+17); // 8 bits por letra + 16 que representa o tamanho no inicio + o bit de paridade no fim
        if(msgcc==NULL) {
            tamanho=-1;
            return resultado;
        }
        paridade=FALSE; // Ao iniciar, o número de '1's é 0, logo a paridade é paridade é falsa (0 é impar)
        for(i=0;i<(tamanho*8)+16;i++){
            msgcc[i]=msg[i]; // Copia o valor do vetor original
            if(msg[i]==TRUE) paridade=Flip(paridade); // Ajusta a partidade
        }
        msgcc[(tamanho*8)+16]=paridade; // Coloca o bit de paridade no final
        resultado.tamanho=(tamanho*8)+17;
        break;
    case 4: // Enquadramento
        /* code */
        break;

    }
  
    resultado.vetor=msgcc;
    free(msg); // Libera a mensagem antiga

    return resultado; //Se algum erro foi encontrado, ele foi retornado durante o código de controle.
}

boolean CamadaDeEnlaceDadosReceptora(boolean* msg){

    msg=CamadaDeEnlaceDadosReceptoraControleDeErro(msg);
    if(msg!=NULL){ // Se a mensagem está correta
        CamadaDeAplicacaoReceptora(msg); // Manda a mensagem para a aplicação
        return TRUE; //Manda um sinal de deu certo para o meio
    }
    //Se não está, manda um sinal de falso que solicita uma retransmissão
    else return FALSE;
}

boolean* CamadaDeEnlaceDadosReceptoraControleDeErro(boolean* msg){

    int i,tamanho;
    boolean paridade;
    //Como, para o método de controle de erros são necessários adicionar/remover bits

    boolean* msgcc; // Representa a mensagem SEM controle

    tamanho=Bits16ToInt(msg);// Captura o tamanho da mensagem

    switch (METODO_DE_ERROS)
    {
    case 1: // CRC - Usando o Polinomio CRC-32
        /* code */
        break;
    case 2: // Paridade Par
        // Para a verificação da partidade par, é necessário um bit ao final da mensagem, que conta se o numero de '1's é par 
        msgcc=(boolean*)malloc(sizeof(boolean)*(tamanho*8)+16); // 8 bits por letra + 16 que representa o tamanho no inicio SEM o bit de paridade no fim
        if(msgcc==NULL) return NULL;
        paridade=TRUE; // Ao iniciar, o número de '1's é 0, logo a paridade é verdadeira
        for(i=0;i<(tamanho*8)+16;i++){
            msgcc[i]=msg[i]; // Copia o valor do vetor original
            if(msg[i]==TRUE) paridade=Flip(paridade); // Ajusta a partidade
        }
        if(msg[(tamanho*8)+16]!=paridade){
//            printf("Erro de paridade detectado!\n");
            return NULL; // Se a partidade calculada, não coincidir com a enviada, houve um erro na transmissão
        } 
        break;
    case 3: // Paridade Impar
        // Para a verificação da partidade par, é necessário um bit ao final da mensagem, que conta se o numero de '1's é impar
        msgcc=(boolean*)malloc(sizeof(boolean)*(tamanho*8)+16); // 8 bits por letra + 16 que representa o tamanho no inicio SEM o bit de paridade no fim
        if(msgcc==NULL) return NULL;
        paridade=FALSE; // Ao iniciar, o número de '1's é 0, logo a paridade é paridade é falsa (0 é impar)
        for(i=0;i<(tamanho*8)+16;i++){
            msgcc[i]=msg[i]; // Copia o valor do vetor original
            if(msg[i]==TRUE) paridade=Flip(paridade); // Ajusta a partidade
        }
        if(msg[(tamanho*8)+16]!=paridade) return NULL; // Se a partidade calculada, não coincidir com a enviada, houve um erro na transmissão
        break;
    case 4: // Enquadramento
        /* code */
        break;
    }

    free(msg); // Libera a mensagem antiga(com bits de controle)

    return msgcc;
}

boolean MeioDeTransmissao (boolean* msg,int tamanhototal) {

    int i;
    float random;

    boolean* fluxoA; // Bits que saem do computador A
    boolean* fluxoB; // Bits que chegam ao computador B

    srand(time(NULL)); // Seta a semente aleatória (para ocorrerem erros de flip de bits)

    //debug

    /*
    printf("tamanho = %d\n",tamanhototal);
    for(i=0;i<tamanhototal;i++){
        if(i%8==0 && i!=0) printf(" ");
        printf("%d",msg[i]);
    } 
    printf("\n");
    */
   
    // O fluxo que sai de A é a mensagem enviada para o Meio (é necessária uma cópia, para não perder os dados originais, caso haja uma mudança no meio)
    // Lembrando que o simulador não tem como foco a performance mas sim, a simulação.
    fluxoA= copiaVetor(msg,tamanhototal); 
    fluxoB=(boolean*)malloc(sizeof(boolean)*tamanhototal);
    
    //Começa a transmitir os dados (8 é o numero de bits de cada caracter e 16 é o numero de bits que representam o tamanho)
    for(i=0;i<tamanhototal;i++){
        //O bit vai sendo normalmente transferido
        fluxoB[i]=fluxoA[i];
        //E aqui a probabilidade de um bit ser 'flipado' entra:
        //Se um numero aleatorio, com 2 casas decimais entre 1 e 100 for menor ou igual que a probabilidade de dar erro, o erro acontece e o bit é flipado
        random=((float)(rand()%100+1)+((float)(rand()%100))/100);
      //  printf("%f ",random);
        if(i>16&&random<=CHANCE_DE_ERRO){
            fluxoB[i]=Flip(fluxoB[i]);
//            printf("Ocorreu um erro no meio de transmissao\n");
        }
    }

 //   printf("fluxoB= ");
 //   for(i=0;i<(tamanho*8)+16;i++) printf("%d",fluxoB[i]);

    //Com a mensagem transmitida, não existe mais fluxo
    free(fluxoB);
    free(fluxoA);

    //Passa os dados para camada de enlace receptora, se ela não detectou erros, ela envia um ack que retorna para a camada transmissora confirmando o envio correto dos dados
    //Se os dados não estavam íntegros na camada de enlace receptora, um sinal é enviado para a camada transmissora, pedindo a retransmissão.
    if(CamadaDeEnlaceDadosReceptora(fluxoB)==TRUE) return TRUE;
    else return FALSE;
    

}

boolean* copiaVetor(boolean* vetorcopiado, int tamanho){

    int i;

    boolean* vetorcopia = (boolean*) malloc(sizeof(boolean)*tamanho);

    for(i=0;i<tamanho;i++){
        vetorcopia[i]=vetorcopiado[i];
    }

    return vetorcopia;

}

boolean* StringToBits(char* mensagem){

    int i,j,tamanhodamensagem;
    boolean* mb; // mb é uma abreviação para 'mensagem em bits'
    boolean* tamanhoembits;

    tamanhodamensagem=strlen(mensagem); // Pega o numero de caracteres da mensagem

    mb=(boolean*)malloc(sizeof(boolean)*(tamanhodamensagem*8)+16); //8 bits por caractere + 16 para o tamanho da mensagem
    if (mb==NULL) return NULL;

    tamanhoembits=IntTo16Bits(tamanhodamensagem); // Transforma o numero de caracteres da mensagem em um numero de 16 bits
    if(tamanhoembits==NULL) return NULL;

    for(i=0;i<16;i++){  // Armazena esse número na mensagem
        mb[i]=tamanhoembits[i];
    }

    free(tamanhoembits); // libera o vetor auxiliar que guardava os tamanhos

    for(i=0;i<tamanhodamensagem;i++){ // Para cara caracter da mensagem (passado os 16 primeiros bits)

        /*
            Faz o and do numero deslocado para esquerda (de 0 a 7 vezes) com 1000 0000, ou seja
            compara, um a um bit do número, se ele for 1 coloca o valor TURE se for 0 coloca o valor FALSE
        */

        for(j=0;j<8;j++){
            if(mensagem[i] << j & 128) mb[(i*8)+j+16] = TRUE; 
            else mb[(i*8)+j+16] = FALSE;
        }

        //debug
        /*
        printf("O codigo para o caracter %c e: ",mensagem[i]);
        for(int k=0;k<8;k++){
            printf("%d",mb[(i*8)+k]);
            if(k==3) printf(" ");
        }
        printf("\n");
        */

    }

    return mb;
}


boolean* IntTo16Bits(int tamanhodamensagem){

    int i;
    int numero=tamanhodamensagem;
    int guardaresto;
    boolean* numero16bits;

    numero16bits=malloc(sizeof(boolean*)*16); // Aloca os 16 bits
    if(numero16bits==NULL) return NULL;

    for(i=15;i>=0;i--){
        if(numero%2==1) numero16bits[i]=1;
        else numero16bits[i]=0;
        numero=numero/2;
    }

    //debug
    //for(i=0;i<16;i++) printf("%d",numero16bits[i]);

    return numero16bits;
}

char* BitsToString(boolean* msg){

    int i,j,tamanho;
    char letra;
    char* mensagem;

    tamanho=Bits16ToInt(msg);

    mensagem=(char*)malloc(sizeof(char)*tamanho+8); // Reserva um para o /0

    j=0;
    for(i=16;i<(tamanho*8)+16;i+=8){
        letra=Bits8ToInt(msg+i);
        mensagem[j]=letra;
        j++;
    }

    mensagem[j]='\0';

    return mensagem;
}

int Bits16ToInt(boolean* int16bits){

    int i,pow2,numero;

    pow2=1;numero=0;
    for(i=15;i>=0;i--){
        if(int16bits[i]==1) numero+=pow2;
        pow2=pow2*2;
    }

    return numero;
}

int Bits8ToInt(boolean* int8bits){

    int i,pow2,numero;

    pow2=1;numero=0;
    for(i=7;i>=0;i--){
        if(int8bits[i]==1) numero+=pow2;
        pow2=pow2*2;
    }

    return numero;
}

boolean Flip(boolean bit){
    if(bit==FALSE) return TRUE;
    else return FALSE;
}