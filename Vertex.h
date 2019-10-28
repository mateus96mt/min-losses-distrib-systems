#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <vector>
#include "Capacitor.h"

using namespace std;

/// Para cálculo do custo em dinheiro por nível de carga
struct LoadLevel{
    double level;   // Nível de carga
    int time;       // Tempo no nível
    double cost;    // Custo por tempo. Em hora, dia, ano, etc.
};

class Edge; // Forward declaration

class Vertex{

private:
    int id;
    int capacitorMax;
    Vertex *nextVertex;
    Edge *edgesList;
    int outdegree, indegree;
    int auxDegree;
    int idTree;
    vector<Capacitor> capacitorsAlloc; // Vetor de tipos capacitores alocados no vertice

    double activePower, reactivePower; // Demandas
    double voltage;
    bool marked;

    vector<LoadLevel> loadFactors;

public:

    static int idLF; //TODO: Private & Static?

    Vertex(int id, int capacitorMaximumAllocation = 1); // Por padrao sera permitido soh um cap, mas podem colocar varios
    ~Vertex();
    void show();
    void addCapacitor( Capacitor newCap );
    void rmLastCapacitor();
    void rm_all_capacitors();
    void chooseFactor(int idFactor);

    //GETS e SETS:
    int getID(){                                return this->id;            };
    Vertex *getNext(){                          return this->nextVertex;    };
    Edge *getEdgesList(){                       return this->edgesList;     };
    double getActivePower(); // * this->loadFactors[this->loadFactor].time; }
    double getReactivePower();
    double getVoltage(){                        return this->voltage;       };
    int getIndegree(){                          return this->outdegree;     };
    int getOutdegree(){                         return this->indegree;      };
    bool getMarked(){                           return this->marked;        };
    int getAuxDegree(){                         return this->auxDegree;     };
    int getIdTree(){                            return this->idTree;        };

    vector<Capacitor> getCapacitors(){          return capacitorsAlloc;     };


    void setID(int id){                         this->id = id;              };
    void setNext(Vertex *v){                    this->nextVertex = v;       };
    void setEdgesList(Edge *e){                 this->edgesList = e;        };
    void setActivePower(double potAt){          this->activePower = potAt;  };
    void setReactivePower(double pow){          this->reactivePower = pow;  };
    void setVoltage(double volt){               this->voltage = volt;       };
    void setLoadFactors(vector<LoadLevel> l){   this->loadFactors = l;      };
    void setOutdegree(int deg){                 this->outdegree = deg;      };
    void setIndegree(int deg){                  this->indegree = deg;       };
    void setMarked(bool mark){                  this->marked = mark;        };
    void setAuxDegree(int deg){                 this->auxDegree = deg;      };
    void setIdTree(int idTree){                 this->idTree = idTree;      };
};
#endif // NO_H_INCLUDED
