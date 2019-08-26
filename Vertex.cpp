#include <stdio.h>

#include "Vertex.h"
#include "Edge.h"

Vertex::Vertex(int id){

    this->id = id;
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

    this->capacitors.clear();
}

Vertex::~Vertex(){
    this->capacitors.clear();
}

double Vertex::getReactivePower(){
    double reactPower = this->reactivePower;
    for( unsigned int i = 0; i < capacitors.size(); i++ ) {
        reactPower -= this->capacitors[i]->getStepPower(0);
//        cout << "CAPACITOR!" << endl;
    }
    return reactPower;
}

void Vertex::addCapacitor( int id, double power, int step ){
    Capacitor *newCap = new Capacitor(id, power, step);
    this->capacitors.push_back( newCap );
}

void Vertex::rmCapacitor(int id){
    this->capacitors.pop_back();
}

void Vertex::show(){
    double capPower = 0;
    for( unsigned int i = 0; i < capacitors.size(); i++ )
        capPower += capacitors[i]->getStepPower(0);

    printf("------------------------------------------------------------------------\n");
    printf("<No{%d}:  idArv = %d,  grauS = %d,  grauE = %d,  grauAUX = %d,  pA = %.5f,  pR = %.5f",
            this->id,this->idTree,this->outdegree,this->indegree,this->auxDegree,this->activePower,this->reactivePower);
    if( capPower > 0 )
        cout << "Capacitor :" << capPower << endl;
    else
        printf("%.5f", this->reactivePower);
    printf("  vol = %.5f >", this->voltage);

    for(Edge *a = this->edgesList; a!=NULL; a= a->getNext()){
        printf("\n\n");
        a->show();
    }
    printf("\n------------------------------------------------------------------------");
    printf("\n\n");
}

