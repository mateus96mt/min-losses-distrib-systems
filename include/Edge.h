#ifndef ARCO_H_INCLUDED
#define ARCO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include "Vertex.h"

class Edge{

private:
    //estrutura basica do Arco:
    int id;
    Edge *nextEdge;
    Vertex *origin, *destiny;

    //informacoes necessarias para o problema de minimizacao de perdas:
    bool closed;//1=chave fechada, ou seja em uso.   0=chave aberta
    double activePowerFlow, reactivePowerFlow;//fluxos de potencia ativa e reativa
    double resistance, reactance;
    double activeLoss, reactiveLoss;//perda de potencia ativa e reativa no arco

    bool modifiable;
    bool marked;
    bool fixed;

public:
    //funcoes do Arco:
    Edge(int id);//construtor
    ~Edge();//destrutor

    void show();



    //GETS e SETS:
    int getID(){              return this->id;    };
    Edge *getNext(){      return this->nextEdge;  };
    Vertex *getOrigin(){        return this->origin; };
    Vertex *getDestiny(){       return this->destiny; };
    bool isClosed(){          return this->closed; };
    double getActivePowerFlow(){  return this->activePowerFlow; };
    double getReactivePowerFlow(){ return this->reactivePowerFlow; };
    double getResistance(){  return this->resistance; };
    double getReactance(){    return this->reactance; };
    double getActiveLoss(){   return this->activeLoss; };
    double getReactiveLoss(){ return this->reactiveLoss; };
    bool getModifiable(){    return this->modifiable; };
    bool getMarked(){        return this->marked;};
    bool getFixed(){          return this->fixed;}

    void setID(int id){                      this->id            = id; };
    void setNext(Edge *arc){ this->nextEdge      = arc; };
    void setOrigin(Vertex *no){ this->origin      = no; };
    void setDestiny(Vertex *no){ this->destiny     = no; };
    void setSwitch(bool chave){ this->closed         = chave; };
    void setActiveFlow(double FpAtiva){ this->activePowerFlow   = FpAtiva; };
    void setReactiveFlow(double FpReativa){ this->reactivePowerFlow = FpReativa; };
    void setResistance(double resist){ this->resistance   = resist; };
    void setReactance(double reat){ this->reactance     = reat; };
    void setActiveLoss(double perdaA){ this->activeLoss    = perdaA; };
    void setReactiveLoss(double perdaReat){ this->reactiveLoss  = perdaReat; };
    void setModifiable(bool modificavel){ this->modifiable   = modificavel; };
    void setMarked(bool marca){ this->marked       = marca;};
    void setFixed(bool fixed){               this->fixed         = fixed;}
};

#endif // ARCO_H_INCLUDED
