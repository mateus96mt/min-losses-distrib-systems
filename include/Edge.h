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

    bool closed;                                // 1 = Closed, 0 = Open
    double activePowerFlow, reactivePowerFlow;  // Fluxos de potencia ativa e reativa
    double resistance, reactance;
    double activeLoss, reactiveLoss;            //perda de potencia ativa e reativa no arco

    bool modifiable;
    bool marked;
    bool fixed;

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
    bool getMarked(){                           return this->marked;                    };
    bool getFixed(){                            return this->fixed;                     };

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
    void setMarked(bool marca){                 this->marked            = marca;        };
    void setFixed(bool fixed){                  this->fixed             = fixed;        };
};

#endif // ARCO_H_INCLUDED
