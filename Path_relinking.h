#ifndef PATH_RELINKING_H_INCLUDED
#define PATH_RELINKING_H_INCLUDED

#include "Graph_network.h"
#include "RK_Individual.h"

class Path_relinking{

private:

    vector <Individuo*> pool;//pool of RK individuals
    int max_it;//maximum iterations without improviment
    float pct_pr_elite; //percentage of best individuals of pool considered elite

public:

    Path_relinking(vector<Individuo*> pool, int max_it, float pct_pr_elite);
    ~Path_relinking(){};

    ///----------------------Path-relingking operators-------------------:

    Individuo *prs(Individuo *inicio,Individuo *guia, Grafo *g, Individuo *indRef);//path relinking simples path de this para guia
    Individuo *prs2(Individuo *inicio, Individuo *guia, Grafo *g);//path relinking com modificacoes sugeridas luciana

    ///----------------------Path-relingking operators-------------------:

    //Current evolutive path-relinking implementation
    Individuo *pre(Grafo *g);
};

#endif // PATH_RELINKING_H_INCLUDED
