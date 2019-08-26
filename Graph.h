#ifndef DIGRAFO_H_INCLUDED
#define DIGRAFO_H_INCLUDED

#include <iostream>
#include <fstream>

#include "Vertex.h"
#include "Edge.h"
#include <vector>
#include <algorithm>

using namespace std;

class Graph{

private:
    //estrutura basica da lista de adjacencia:
    Vertex *verticesList;
    int verticesSize, edgesSize;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double totalActiveLoss, totalReactiveLoss;

    //informacoes uteis:
    int num_markeds; //contador de nos marcados, util em percursos em profundidade

    //Limites para o uso de capacitores
    int maxCapacitorsBus;       // num. max de capacitores em cada barra
    int maxCapacitorNetwork;    // num. max de capacitores em toda a rede

public:
    //funcoes do grafo:
    Graph();//construtor
    ~Graph();//destrutor

    Vertex* findVertex(int id);
    Edge *findEdge(int id);
    Edge *findEdge(int originID, int destinyID);


    void insertVertex(int id, double activePower, double reactivePower, double voltage);
    void insertEdge(int originID, int destinyID, int id, double resist, double react, bool swit);

    void input_read(string name);

    void printGraph();


    //calcula o somatorio de cargas e perdas ativas para o ramo abaixo do no "no"
    double branchActiveLoss(Vertex *no);
    void auxBranchActiveLoss(Vertex *no, double &soma);

    //calcula o somatorio de cargas e perdas reativas para o ramo abaixo do no "no"
    double branchReactiveLoss(Vertex *no);
    void auxBranchReactiveLoss(Vertex *no, double &soma);

    void forward(int it);
    void auxForward(Vertex *no, Edge *ak, int it);

    void backward();
    void auxBackward(Vertex *no);

    double * getLosses();
    void evaluateLossesAndFlows(double tol);

    double minVoltage();


    void unmarkVertices();//util em percursos

    bool isTree();
    void auxIsTree(Vertex *no, int &marcados, bool &ciclo);

    bool isConected();
    void auxIsConnected(Vertex *no, int &n_marcados);

    void defineFlows();
    void auxDefineFlows(Vertex *no, Vertex *noAnterior);

    void defineModifiables();

    Graph *returnCopy();

    void randomSolution();

    void resetAuxDegrees();
    void resetIDTree();
    void resetFlowAndLoss();



    //GETS e SETS:
    Vertex *get_verticesList(){             return this->verticesList;          };
    int getVerticesSize(){                  return this->verticesSize;          };
    int getEdgesSizes(){                    return this->edgesSize;             };
    int getNumMarkeds(){                    return this->num_markeds;           };
    int getMaxCapacitorsBus(){              return this->maxCapacitorsBus;      };
    int getMaxCapacitorNetwork(){           return this->maxCapacitorNetwork;   };

    void setVerticesList(Vertex *no){       this->verticesList = no;            };
    void setMaxCapacitorsBus(int m){        this->maxCapacitorsBus = m;         };
    void setMaxCapacitorNetwork(int m){     this->maxCapacitorNetwork = m;      };
};

#endif // DIGRAFO_H_INCLUDED
