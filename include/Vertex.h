#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED

#include "Capacitor.h"
#include <vector>

using namespace std;

struct LoadLevel{
    double level;   // Nível de carga
    int time;       // Tempo no nível
    double cost;    // Custo por tempo. Em hora, dia, ano, etc.
};

class Edge;

class Vertex{

private:
    //estrutura basica do No
    int id;
    int capacitorMax;
    Vertex *nextVertex;
    Edge *edgesList;
    int outdegree, indegree;
    int auxDegree;
    int idTree;
    vector<Capacitor> capacitorsAlloc;

    //informacoes necessarias para o problema de minimizacao de perdas:
    double activePower, reactivePower;//demandas
    double voltage;

    //informacoes uteis:
    bool marked;

    vector<LoadLevel> loadFactors;

public:
    static int idLF; //TODO: Private & Static?

    //Funcoes do No:
    Vertex(int id, int capacitorMaximumAllocation = 1);//construtor
    ~Vertex();//destrutor

    void show();
    void addCapacitor( Capacitor newCap );
    void rmLastCapacitor();
    void rm_all_capacitors();
    void chooseFactor(int idFactor);

    //GETS e SETS:
    int getID(){            return this->id;    };
    Vertex *getNext(){        return this->nextVertex; };
    Edge *getEdgesList(){  return this->edgesList; };
    double getActivePower(){   return this->activePower; };
    double getReactivePower(){ return this->reactivePower; };
    double getVoltage(){   return this->voltage; };
    int getIndegree(){     return this->outdegree; };
    int getOutdegree(){   return this->indegree; };
    bool getMarked(){      return this->marked; };
    int getAuxDegree(){       return this->auxDegree; };
    int getIdTree(){         return this->idTree;};

    vector<Capacitor> getCapacitors(){          return capacitorsAlloc;     };

    void setID(int id){                 this->id          = id; };
    void setNext(Vertex *no){ this->nextVertex      = no; };
    void setEdgesList(Edge *Arc){ this->edgesList  = Arc; };
    void setActivePower(double potAt){ this->activePower    = potAt; };
    void setReactivePower(double potReat){ this->reactivePower  = potReat; };
    void setVoltage(double volt){ this->voltage    = volt; };
    void setLoadFactors(vector<LoadLevel> l){   this->loadFactors = l;      };
    void setOutdegree(int grau){ this->outdegree   = grau;};
    void setIndegree(int grau){ this->indegree = grau; };
    void setMarked(bool marca){ this->marked     = marca; };
    void setAuxDegree(int grau){ this->auxDegree     = grau;};
    void setIdTree(int idArv){ this->idTree       = idArv;};

};

#endif // NO_H_INCLUDED
