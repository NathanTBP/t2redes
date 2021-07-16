/* Objetivo: Fazer um programa que simule o envio um dado de um computador A para um computador computador B 
realizando as devidas conversões entre String, da mensagem a ser enviada, e os bytes e bits a serem
verificados pela camada de enlace.
*/
#include <stdio.h>       // Para entrada/saida (leitura e escrita via teclado monitor e arquivos)
#include <stdlib.h>      // Para alocação dinâmica
#include <string.h>      // Para manipular strings e setar memória
#include <time.h>//necessário p/ função time()

//Definição de um tipo booleano para C -> Unsigned char é a menor unidade de dados possível na linguagem.
#define boolean unsigned char 
#define TRUE 1
#define FALSE 0

//definição de máximos e mínimos para 2 valores
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

boolean AplicacaoTransmissora(void);
boolean AplicacaoReceptora(char* mensagem);
boolean CamadaDeAplicacaoTransmissora(char* mensagem);
boolean CamadaDeAplicacaoReceptora(boolean* msg);
boolean* StringToBits(char* mensagem);
boolean* IntTo16Bits(int tamanhodamensagem);
char* BitsToString(boolean* msg);
int Bits16ToInt(boolean* intem16bits);
boolean CamadaDeEnlaceDadosTransmissora(boolean* msg);
boolean CamadaDeEnlaceDadosTransmissoraControleDeErro(boolean* msg);
boolean CamadaDeEnlaceDadosReceptora(boolean* msg);
boolean CamadaDeEnlaceDadosReceptoraControleDeErro(boolean* msg);
boolean MeioDeTransmissao(boolean* msg);

unsigned int METODO_DE_ERROS = 1;
unsigned int CHANCE_DE_ERRO = 1;
unsigned int TAMANHO_MAXIMO_DA_MENSAGEM = 100;
unsigned int TENTATIVAS_REENVIO = 5;

/* O objetivo principal é transmitir os dados

Relembrando as camadas da rede e suas principais funções:

Aplicação - Interface entre usuário e rede
Transporte - Conectar processos
Rede - Conectar máquinas
Enlace - Terminar o pacote e o enviar ao nó adjacente
Física - Transmitir, em algum meio físico, os dados.

No trabalho anterior, já foi tratado de portas e protocolos que englobam as camadas de transporte e rede, para esse trabalho,
é considerado que isso já está feito e os dados vem de um usuário e passam direto a camada de enlace.

*/

//O programa principal faz a interface com o usuário, para que ele defina as variáveis envolvidas na transmissão da mensagem
int main (void){

    int variaveldomenu;
    boolean loop=TRUE;

    printf("Ola! Bem vindo(a) ao simulador de transmissão de dados v1.0.1\n");
    printf("1: Iniciar Programa!\n");
    printf("2: Metodo de controle de erros [1=CRC,2=Bit Impar,3=Bit Par,4=Enquadramento] (Atual = %d)\n",METODO_DE_ERROS);
    printf("3: Porcentagem de chance de ocorrer um erro em cada bit da transmissao (Atual = %d%%)\n",CHANCE_DE_ERRO);
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
         else printf("Ocorreu um erro e a mensagem não pode ser enviada\n");
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
        scanf(" %d",&CHANCE_DE_ERRO);
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
        printf("3: Porcentagem de chance de ocorrer um erro em cada bit da transmissao (Atual = %d%%)\n",CHANCE_DE_ERRO);
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

    //A aplicação recptora quer, receber uma string e imprimi-la na tela.

    if(mensagem!=NULL) { // Se a mensagem é uma string de fato, a imprime
    fputs(mensagem,stdout);
    return TRUE; // Deu tudo certo
    }

    return FALSE; // Retorna erro

}

boolean CamadaDeAplicacaoTransmissora(char* mensagem){

    //A camada de Aplicação transmissora quer, receber uma string da aplicação, transformá-la em bits e enviá-la a próxima camada
    //Que usualmente é a camada de Transporte, porém, neste trabalho, é diretamente a de enlace

    boolean* msg; // Como são bits, os valores válidos são 0 e 1 (true ou falso)
    boolean deucerto=FALSE;

    //Função que converte uma string para um grande array de bits (os 16 primeiros bits são o tamanho)
    msg=StringToBits(mensagem);

    if(msg!=NULL){
        
        //Nesse momento, são considerados que os endereços estão certos, apenas o dado precisa ser enviado
        // Logo, a esse "pulo" direto entre as camadas de aplicação e enlace
        if(CamadaDeEnlaceDadosTransmissora(msg)) deucerto=TRUE;
    
    }
    //Libera a mensagem (binária)
    free(msg);
    if(deucerto==TRUE) return TRUE;
    else return FALSE; // Ocorreu algum erro

}

boolean CamadaDeAplicacaoReceptora(boolean* msg){

    char* mensagem;

    //Função que converte um grande array de bits para uma string (os 16 primeiros bytes são o tamanho)
    mensagem=BitsToString(msg);

    if(mensagem!=NULL){ // Se a mensagem foi recebida corretamente
        
        AplicacaoReceptora(mensagem); // Passa a mensagem para a aplicação
        return TRUE; // Avisa que deu tudo certo
    
    }

    return FALSE; // Ocorreu algum erro

}

boolean CamadaDeEnlaceDadosTransmissora(boolean* msg){

    int tentativas = TENTATIVAS_REENVIO;

    //Chama o controle de Erros
    if(CamadaDeEnlaceDadosTransmissoraControleDeErro(msg)){ // Se o erro foi filtrado, pode chamar a camada física (meio de transmissão)
        
        while (tentativas>0){ // Faz o número de tentativas necessárias a função que envia os dados, se nenhuma der certo, retorna um erro
            if(MeioDeTransmissao(msg)==TRUE) return TRUE; // Se a mensagem foi enviada com sucerro, retorna sucesso
            tentativas--;
        }
        
    } 

        return FALSE; // Ocorreu um erro

}


boolean CamadaDeEnlaceDadosTransmissoraControleDeErro(boolean* msg){

    switch (METODO_DE_ERROS)
    {
    case 1: // CRC
        /* code */
        break;
    case 2: // Paridade Par
        /* code */
        break;
    case 3: // Paridade Impar
        /* code */
        break;
    case 4: // Enquadramento
        /* code */
        break;

    }

    return FALSE;
}

boolean CamadaDeEnlaceDadosReceptora(boolean* msg){

       if(CamadaDeEnlaceDadosReceptoraControleDeErro(msg)==FALSE){ // Se não houveram erros
       CamadaDeAplicacaoReceptora(msg); // Manda a mensagem para a aplicação
       return TRUE; //Manda um sinal de deu certo para o meio
       }
       else return FALSE;
}

boolean CamadaDeEnlaceDadosReceptoraControleDeErro(boolean* msg){

    switch (METODO_DE_ERROS)
    {
    case 1: // CRC
        /* code */
        break;
    case 2: // Paridade Par
        /* code */
        break;
    case 3: // Paridade Impar
        /* code */
        break;
    case 4: // Enquadramento
        /* code */
        break;
    }

    return FALSE;
}

boolean MeioDeTransmissao (boolean* msg) {

    int erro,tamanho,i;

    boolean* fluxoA;
    boolean* fluxoB;

    srand(time(NULL)); // Seta a semente aleatória

    tamanho=Bits16ToInt(msg);// Captura o tamanho da mensagem

    // O fluxo que sai de A é a mensagem enviada para o Meio (é necessária uma cópia, para não perder os dados originais, caso haja uma mudança no meio)
    // Lembrando que o simulador não tem como foco a performance mas sim, a simulação.
    fluxoA= copiaVetor(msg,(tamanho*8)+16); 
    fluxoB=(boolean*)malloc(sizeof(boolean)*(tamanho*8)+16);
    
    //Coemeça a transmitir os dados
    for(i=16;i<(tamanho*8)+16;i++){
        //Se um numero aleatorio entre 1 e 100 for maior que a probabilidade, nada acontece
        if((rand()%100+1)>CHANCE_DE_ERRO) fluxoB[i]=fluxoA[i];
        //Porém, se não for, ocorreu um erro no bit e esse bit é flipado
        else if(fluxoA[i]==TRUE) fluxoB[i]=FALSE;
        else fluxoB[i]=TRUE;
    }

    //Passa os dados para camada de enlace receptora, se ela não detectou erros, ela envia um ack que retorna para a camada transmissora
    if(CamadaDeEnlaceDadosReceptora(fluxoB)==TRUE){
        return TRUE;
    }
    //Se os dados não estavam íntegros na camada de enlace receptora, um sinal é enviado para a camada transmissora, pedindo a retransmissão.
    else return FALSE;

}

boolean* copiaVetor(boolean* vetorcopiado, int tamanho){

    int i;

    boolean* vetorcopia = (boolean*) malloc (sizeof(boolean)*tamanho);

    for(i=0;i<tamanho;i++){
        vetorcopia[i]=vetorcopiado[i];
    }

    return vetorcopiado;

}

boolean* StringToBits(char* mensagem){

    int i,j,tamanhodamensagem;
    boolean* mb; // mb é uma abreviação para mensagem em bits
    boolean* tamanhoembits;

    tamanhodamensagem=strlen(mensagem);

    mb=(boolean*)malloc(sizeof(boolean)*(tamanhodamensagem*8)+16); //8 bits por caractere + 16 para o tamanho da mensagem
    if (mb==NULL) return NULL;

    tamanhoembits=IntTo16Bits(tamanhodamensagem);
    if(tamanhoembits==NULL) return NULL;

    for(i=0;i<16;i++){
        mb[i]=tamanhoembits[i];
    }

    for(i=2;i<tamanhodamensagem;i++){ // Para cara caracter da mensagem

        /*
            Faz o and do numero deslocado para esquerda (de 0 a 7 vezes) com 1000 0000, ou seja
            compara, um a um bit do número, se ele for 1 coloca o valor TURE se for 0 coloca o valor FALSE
        */

        for(j=0;j<8;j++){
            if(mensagem[i] << j & 128) mb[(i*8)+j] = TRUE; 
            else mb[(i*8)+j] = FALSE;
        }

        /* debug
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
    boolean* numero16bits;

    numero16bits=malloc(sizeof(boolean*)*16);
    if(numero16bits==NULL) return NULL;

    for(i=0;i<16;i++){

    }

}

char* BitsToString(boolean* msg){

   

    return NULL;
}

int Bits16ToInt(boolean* intem16bits){

    int i,pow2;

    for(i=0;i<16;i++){

    }

    return 1;
}