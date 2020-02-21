//
// Created by mateus on 09/08/2019.
//

#ifndef MIN_PERDAS_INDIVIDUAL_H
#define MIN_PERDAS_INDIVIDUAL_H

#include "include/Graph.h"
#include "RK_Individual.h"

class Individual{
private:
    int size;
    vector<Edge*> openedSwitches;
    double activeLoss, reactiveLoss;
public:
    Individual(){};
    Individual(Individual *os);
    Individual(RK_Individual *ind, Graph *g);
    Individual(int *idsAbertos, int n, Graph *g);
    ~Individual(){};

    //GETs
    int getSize(){return size;}
    vector<Edge*> &getOpenedSwitches(){return this->openedSwitches;}
    double getActiveLoss(){return this->activeLoss;}
    double getReactiveLoss(){return this->reactiveLoss;}

    //SETs
    void setOpenSwitches(int position, Edge *e){this->openedSwitches.at(position)=e;}

    void openSwitchesInNetwork(Graph *g);
    void calcObjectiveF(Graph *g);
    void imprime();

    RK_Individual *OS_to_RK(Graph *g);

    bool checkSolution(Graph *g);


    vector<int> evaluate(Edge *e, Graph *g);

};
#endif //MIN_PERDAS_INDIVIDUAL_H


