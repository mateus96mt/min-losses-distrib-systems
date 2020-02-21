#include <stdio.h>

#include "include/Vertex.h"
#include "include/Edge.h"

Vertex::Vertex(int id, int capacitorMaximumAllocation ){

    this->id = id;
    this->capacitorMax = capacitorMaximumAllocation;
    this->nextVertex = NULL;
    this->edgesList = NULL;

    this->activePower = 0.0;
    this->reactivePower = 0.0;
    this->voltage = 1.0;
    this->marked = false;
    this->indegree = 0;
    this->outdegree = 0;
    this->auxDegree = 0;
    this->idTree = this->id;
}

Vertex::~Vertex(){
}

void Vertex::show(){
    printf("------------------------------------------------------------------------\n");
    printf("< No{%d}  idArv = %d  grauS = %d  grauE = %d  grauAUX = %d  pA = %.5f  pR = %.5f  vol = %.5f >",
           this->id, this->idTree, this->outdegree, this->indegree, this->auxDegree, this->activePower, this->reactivePower, this->voltage);

    for(Edge *a = this->edgesList; a != NULL; a= a->getNext()){
        printf("\n\n");
        a->show();
    }
    printf("\n------------------------------------------------------------------------");
    printf("\n\n");
}

void Vertex::addCapacitor( Capacitor newCap ){
    capacitorsAlloc.push_back( newCap );
}

void Vertex::rmLastCapacitor(){
    this->capacitorsAlloc.pop_back();
}

void Vertex::rm_all_capacitors(){
    while(!capacitorsAlloc.empty())
        this->rmLastCapacitor();
}