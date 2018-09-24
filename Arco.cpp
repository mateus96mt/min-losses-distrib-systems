#include <stdio.h>
#include "Arco.h"

Arco::Arco(int id){

    this->id = id;

    this->proxArco = NULL;
    this->noOrigem =NULL;
    this->noDestino =NULL;

    this->chave = true;

    this->FLuxoPAtiva = 0.0;
    this->FLuxoPReativa = 0.0;

    this->resistencia = 0.0;
    this->reatancia = 0.0;

    this->perdaAtiva = 0.0;
    this->perdaReativa = 0.0;

    this->modificavel = true;
    this->marcado = true;
}

Arco::~Arco(){

}

void Arco::imprime(){
    printf("[ ( %d, %d ) A{%d} chave = %d  modif = %d res = %.5f  reat = %.5f  fA = %.5f  fR = %.5f  peA = %.9f  peR = %.9f ]",
    this->noOrigem->getID(), this->noDestino->getID(), this->id, this->chave, this->modificavel, this->resistencia, this->reatancia,
    this->FLuxoPAtiva, this->FLuxoPReativa, this->perdaAtiva, this->perdaReativa);
}
