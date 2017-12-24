#include <stdio.h>

#include "No.h"
#include "Arco.h"

No::No(int id){

    this->id = id;

    this->proxNo = NULL;
    this->listaArcos = NULL;

    this->potAtiva = 0.0;
    this->potReativa = 0.0;
    this->voltagem = 1.0;

    this->marcado = false;

    this->grauEntrada = 0;
    this->grauSaida = 0;

}

No::~No(){
}

void No::imprime(){
    printf("------------------------------------------------------------------------\n");
    printf("< No{%d}  grauS = %d  grauE = %d  pA = %.5f  pR = %.5f  vol = %.5f >",
    this->id, this->grauSaida, this->grauEntrada, this->potAtiva, this->potReativa, this->voltagem);

    for(Arco *a = this->listaArcos; a!=NULL; a=a->getProxArco()){
        printf("\n\n");
        a->imprime();
    }
    printf("\n------------------------------------------------------------------------");
    printf("\n\n");
}
