#ifndef DIGRAFO_H_INCLUDED
#define DIGRAFO_H_INCLUDED

#include <iostream>
#include <fstream>

#include "Vertex.h"
#include "include/Edge.h"
#include <vector>
#include <algorithm>
#include "Capacitor.h"

using namespace std;

class Graph{

private:
    //estrutura basica da lista de adjacencia:
    Vertex *verticesList;
    int verticesSize, edgesSize;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double totalActiveLoss, totalReactiveLoss;

    int maxCapacitorsBus;               // num. max de capacitores em cada barra
    vector<Capacitor> capacitorType;    // tipos de capacitores na rede

    double VB, PB, ZB, mw_factor = 1e-3;

    vector<LoadLevel> loads = vector<LoadLevel>();
    int idFac = 0;

    //informacoes uteis:
    int numberOfMarked; //contador de nos marcados, util em percursos em profundidade
    int numberOfNonModifiable;

public:
    Graph();//construtor
    ~Graph();//destrutor

    Vertex* findVertex(int id);
    Edge *findEdge(int id);
    Edge *findEdge(int originID, int destinyID);

    void insertVertex( int id, double activePower, double reactivePower, double voltage, int idLoadFactor = 1 ); //TODO: Pode ser geográfico os fatores de carga por isso o último param
    void insertEdge( int originID, int destinyID, int id, double resist, double react, bool swit);
    void createCapacitorType(int id, double reactive_power, double cost_per_KVAr,  int step = 0);

    void leEntrada(char nome[]);
    void input_read( string name );

    void show_losses( double powerLoss, double minTension, int idLoad = 0 );

    //calcula o somatorio de cargas e perdas ativas para o ramo abaixo do no "no"
    double branchActiveLoss(Vertex *no);
    void auxBranchActiveLoss(Vertex *no, double &soma);

    //calcula o somatorio de cargas e perdas reativas para o ramo abaixo do no "no"
    double branchReactiveLoss(Vertex *v);
    void auxBranchReactiveLoss(Vertex *v, double &sum);

    void forward(int it);
    void auxForward(Vertex *vertex, Edge *edge, int it);
    void backward();
    void auxBackward(Vertex *vertex);

    double *getLosses();

    /** funcao que soma as perdas e reseta os arcos
    utilizada para o calculo de funcao objetivo otimizado do individuo
    **/
    double *getLossesReseting();

    double evaluateLossesAndFlows(double tol, int idLoadFactorGlobal = 0);

    Vertex *greedyCapacitorAllocation( );

    double minVoltage();

    void unmarkVertices();

    bool isTree();
    void auxIsTree(Vertex *v, int &markeds, bool &isCycle);

    bool isConected();
    void auxIsConnected(Vertex *v, int &n_markeds);

    void defineFlows();
    void auxDefineFlows(Vertex *v, Vertex *v_previous);

    void defineModifiables();

    Graph *returnCopy();

    void randomSolution();

    void resetAuxDegrees();
    void resetIDTree();
    void resetFlowAndLoss();
    void capacitor_allocation();

    void closeNotModifiableEdges();
    void resetModifiables();

    void resetEdgesMarking();
    void markOneDirectionInEdges();

    void deepSearchPath(Vertex *start, int value);

    void checkCycle();

    //GETS e SETS:
    Vertex *get_verticesList(){                     return this->verticesList; };
    int getVerticesSize(){                          return this->verticesSize; };
    int getEdgesSizes(){                            return this->edgesSize; };
    int getNumMarkeds(){                            return this->numberOfMarked; };
    int getNumberOfNonModifiable(){                 return this->numberOfNonModifiable;};
    int getMaxCapacitorsBus(){                      return this->maxCapacitorsBus;                  };
    Capacitor getCapacitorType( int type ){         return this->capacitorType[type-1];             };
    vector<LoadLevel> getLoads(){                   return this->loads;                             };

    void set_verticesList(Vertex *no){ this->verticesList = no; };
    void markConexeComponent(Vertex *start, int value);
    void aux_markConexeComponent(Vertex *start, Vertex *previous, int value);
    double * getTotalLoss();

    void setIdFac( int idF ){                       this->idFac = idF;                              };
    void resetIdFac( int idF ){                     this->idFac = 0;                                };
    void setVerticesList(Vertex *v){                this->verticesList = v;                         };
    void setMaxCapacitorsBus(int m){                this->maxCapacitorsBus = m;                     };

    void addChargeLevel( double level, int time, double cost ){ this->loads.push_back( (LoadLevel) { level, time, cost } );  };
};

#endif // DIGRAFO_H_INCLUDED
