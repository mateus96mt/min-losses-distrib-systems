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
    this->grauAux = 0;

    this->idArv = this->id;

    this->capacitores.clear();
}

No::~No(){
    this->capacitores.clear();
}


double No::getPotReativa(){
    potReativa = this->potReativa;
    for( unsigned int i = 0; i < capacitores.size(); i++ )
        potReativa -= capacitores[i].getStep();
    return potReativa;
}

void No::imprime(){
    int cap = 0;
    for( unsigned int i = 0; i < capacitores.size(); i++ )
        cap += capacitores[i].getStep();

    printf("------------------------------------------------------------------------\n");
    printf("< No{%d}  idArv = %d  grauS = %d  grauE = %d  grauAUX = %d  pA = %.5f  pR = ",
            this->id, this->idArv, this->grauSaida, this->grauEntrada, this->grauAux, this->potAtiva);
    if( cap > 0 )
        printf("%.5f (%.5f - %.5f)", this->potReativa - cap, this->potReativa, cap );
    else
        printf("%.5f", this->potReativa);
    printf("  vol = %.5f >", this->voltagem);

    for(Arco *a = this->listaArcos; a!=NULL; a=a->getProxArco()){
        printf("\n\n");
        a->imprime();
    }
    printf("\n------------------------------------------------------------------------");
    printf("\n\n");
}
