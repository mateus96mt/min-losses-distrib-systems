#ifndef ARCO_H_INCLUDED
#define ARCO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include "Vertex.h"

class Edge{

private:
    int id;
    Edge *nextEdge;
    Vertex *origin, *destiny;

    /// Informacoes para o problema de minimizacao de perdas:
    bool closed;                                // 1 = Closed, 0 = Open
    double activePowerFlow, reactivePowerFlow;  // Fluxos de potencia ativa e reativa
    double resistance, reactance;
    double activeLoss, reactiveLoss;            //perda de potencia ativa e reativa no arco

    bool modifiable;

public:
    Edge(int id);
    ~Edge();

    void show();

    int getID(){                                return this->id;                        };
    Edge *getNext(){                            return this->nextEdge;                  };
    Vertex *getOrigin(){                        return this->origin;                    };
    Vertex *getDestiny(){                       return this->destiny;                   };
    bool isClosed(){                            return this->closed;                    };
    double getActivePowerFlow(){                return this->activePowerFlow;           };
    double getReactivePowerFlow(){              return this->reactivePowerFlow;         };
    double getResistance(){                     return this->resistance;                };
    double getReactance(){                      return this->reactance;                 };
    double getActiveLoss(){                     return this->activeLoss;                };
    double getReactiveLoss(){                   return this->reactiveLoss;              };
    bool getModifiable(){                       return this->modifiable;                };

    void setID(int id){                         this->id                = id;           };
    void setNext(Edge *edge){                   this->nextEdge          = edge;         };
    void setOrigin(Vertex *vertex){             this->origin            = vertex;       };
    void setDestiny(Vertex *vertex){            this->destiny           = vertex;       };
    void setSwitch(bool state){                 this->closed            = state;        };
    void setActiveFlow(double activeFlow){      this->activePowerFlow   = activeFlow;   };
    void setReactiveFlow(double reactiveFlow){  this->reactivePowerFlow = reactiveFlow; };
    void setResistance(double resist){          this->resistance        = resist;       };
    void setReactance(double react){            this->reactance         = react;        };
    void setActiveLoss(double loss){            this->activeLoss        = loss;         };
    void setReactiveLoss(double loss){          this->reactiveLoss      = loss;         };
    void setModifiable(bool modifiable){        this->modifiable        = modifiable;   };
};

#endif // ARCO_H_INCLUDED
