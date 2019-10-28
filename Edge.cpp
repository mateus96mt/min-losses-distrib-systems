#include <stdio.h>
#include "Edge.h"

Edge::Edge(int id){

    this->id = id;

    this->nextEdge          = NULL;
    this->origin            = NULL;
    this->destiny           = NULL;

    this->closed            = true;

    this->activePowerFlow   = 0.0;
    this->reactivePowerFlow = 0.0;
    this->resistance        = 0.0;
    this->reactance         = 0.0;
    this->activeLoss        = 0.0;
    this->reactiveLoss      = 0.0;

    this->modifiable = true;
}

Edge::~Edge(){
}

void Edge::show(){
    printf("[ ( %d, %d ) A{%d} chave = %d  modif = %d res = %.5f  reat = %.5f  fA = %.5f  fR = %.5f  peA = %.9f  peR = %.9f ]",
    this->origin->getID(), this->destiny->getID(), this->id, this->closed, this->modifiable, this->resistance, this->reactance,
    this->activePowerFlow, this->reactivePowerFlow, this->activeLoss, this->reactiveLoss);
}
