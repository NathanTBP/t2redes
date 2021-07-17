#include <stdio.h>
#include <stdlib.h>

#define boolean unsigned char 
#define TRUE 1
#define FALSE 0


int main(void){

    int i;
    int numero;
    int guardaresto;
    boolean* numero16bits;

    scanf(" %d",&numero);

    numero16bits=malloc(sizeof(boolean*)*16); // Aloca os 16 bits
    if(numero16bits==NULL) return NULL;

    for(i=15;i>=0;i--){
        if(numero%2==1) numero16bits[i]=1;
        else numero16bits[i]=0;
        numero=numero/2;
    }

    for(i=0;i<16;i++) printf("%d",numero16bits[i]);

    int i2,pow2,numero2;

    pow2=1;numero2=0;
    for(i2=15;i2>=0;i2--){
        if(numero16bits[i2]==1) numero2+=pow2;
        pow2=pow2*2;
    }

    printf("\n%d\n",numero2);

    return 0;
}