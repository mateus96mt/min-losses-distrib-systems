#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

#include <vector>

using namespace std;

class Capacitor {
private:

    int id;
    double custo;               // custo do capacitor
    int maxCapacitoresBarra;    // num. max de capacitores alocados em uma mesma barra

    /** fluxos de potencia utilizados nesta alocacao ao longo do dia sendo que cada periodo do dia ocupa uma posicao
     * do vetor. Caso n�o seja utilizado chaveamento, o fluxo de potencia utilizado ficar� na posic�o 0 do vetor **/
    vector<double> powerSteps;

public:
    Capacitor(int id, double power, int step = 0){
        this->id = id;
        this->powerSteps.insert( this->powerSteps.begin(), power);
    };
    ~Capacitor(){};

    double getStepPower(int step = 0){                      return powerSteps[step];            }

    int getId() const{                                      return this->id;                    }
    double getCusto() const{                                return this->custo;                 }
    int getMaxCapacitoresBarra() const{                     return this->maxCapacitoresBarra;   }

    void setId(int id){                                     this->id = id;                                  }
    void setCusto(double custo){                            this->custo = custo;                            }
    void setMaxCapacitoresBarra(int maxCapacitoresBarra){   this->maxCapacitoresBarra = maxCapacitoresBarra;}
};


/** Funcao objetivo:
 * - custo da perda
 * - custo do capacitor
 * - reconfiguracao (chaveamento) **/

class Edge;

class Vertex{

private:
    int id;
    Vertex *nextVertex;
    Edge *edgesList;
    int outdegree, indegree;
    int auxDegree;
    int idTree;
    vector<Capacitor*> capacitors; // Vetor de capacitores alocados no vertice

    double activePower, reactivePower; // Demandas
    double voltage;

    bool marked;

public:
    Vertex(int id);
    ~Vertex();
    void show();
    void addCapacitor(int id, double power, int step = 0);
    void rmCapacitor(int id);

    //GETS e SETS:
    int getID(){                            return this->id;            };
    Vertex *getNext(){                      return this->nextVertex;    };
    Edge *getEdgesList(){                   return this->edgesList;     };
    double getActivePower(){                return this->activePower;   };
    double getReactivePower();
    double getVoltage(){                    return this->voltage;       };
    int getIndegree(){                      return this->outdegree;     };
    int getOutdegree(){                     return this->indegree;      };
    bool getMarked(){                       return this->marked;        };
    int getAuxDegree(){                     return this->auxDegree;     };
    int getIdTree(){                        return this->idTree;        };

    vector<Capacitor*> getCapacitors(){    return capacitors;         };


    void setID(int id){                     this->id = id;              };
    void setNext(Vertex *v){                this->nextVertex = v;       };
    void setEdgesList(Edge *e){             this->edgesList = e;        };
    void setActivePower(double potAt){      this->activePower = potAt;  };
    void setReactivePower(double pow){      this->reactivePower = pow;  };
    void setVoltage(double volt){           this->voltage = volt;       };
    void setOutdegree(int deg){             this->outdegree = deg;      };
    void setIndegree(int deg){              this->indegree = deg;       };
    void setMarked(bool mark){              this->marked = mark;        };
    void setAuxDegree(int deg){             this->auxDegree = deg;      };
    void setIdTree(int idTree){             this->idTree = idTree;      };
};
#endif // NO_H_INCLUDED
