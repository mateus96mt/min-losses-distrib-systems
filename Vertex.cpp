#include <stdio.h>

#include "Vertex.h"
#include "Edge.h"

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
    this->grauAux = 0;

    this->idArv = this->id;
}

No::~No(){
}

void No::imprime(){
    printf("------------------------------------------------------------------------\n");
    printf("< No{%d}  idArv = %d  grauS = %d  grauE = %d  grauAUX = %d  pA = %.5f  pR = %.5f  vol = %.5f >",
    this->id, this->idArv, this->grauSaida, this->grauEntrada, this->grauAux, this->potAtiva, this->potReativa, this->voltagem);

    for(Arco *a = this->listaArcos; a!=NULL; a=a->getProxArco()){
        printf("\n\n");
        a->imprime();
    }
    printf("\n------------------------------------------------------------------------");
    printf("\n\n");
}
