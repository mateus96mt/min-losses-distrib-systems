//
// Created by mateus on 09/08/2019.
//

#ifndef MIN_PERDAS_OS_INDIVIDUAL_H
#define MIN_PERDAS_OS_INDIVIDUAL_H

#include "include/Graph_network.h"
#include "RK_Individual.h"

class OS_Individual{
private:
    int size;
    vector<Edge*> openedSwitches;
    double activeLoss, reactiveLoss;
public:
    OS_Individual(){};
    OS_Individual(OS_Individual *os);
    OS_Individual(RK_Individual *ind, Graph_network *g);
    OS_Individual(int *idsAbertos, int n, Graph_network *g);
    ~OS_Individual(){};

    //GETs
    int getSize(){return size;}
    vector<Edge*> &getOpenedSwitches(){return this->openedSwitches;}
    double getActiveLoss(){return this->activeLoss;}
    double getReactiveLoss(){return this->reactiveLoss;}

    //SETs
    void setOpenSwitches(int position, Edge *e){this->openedSwitches.at(position)=e;}

    void openSwitchesInNetwork(Graph_network *g);
    void calcObjectiveF(Graph_network *g);
    void imprime();

    RK_Individual *OS_to_RK(Graph_network *g);

    bool checkSolution(Graph_network *g);


    vector<int> evaluate(Edge *e, Graph_network *g);

};
#endif //MIN_PERDAS_OS_INDIVIDUAL_H


