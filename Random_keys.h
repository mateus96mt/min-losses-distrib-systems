#ifndef RANDOM_KEYS_H_INCLUDED
#define RANDOM_KEYS_H_INCLUDED

#include "Grafo.h"

struct cromossomo{
    Arco *a;
    double peso;
};

vector<cromossomo*> geraSolucaoAleatoria(Grafo *g);
double *perdaTotalSolucao(vector<cromossomo*> solucao, Grafo *g);
vector<cromossomo*> cruzamentoMedia(vector<cromossomo*> s1, vector<cromossomo*> s2);
void mutacao1(vector<cromossomo*> &solucao, int chance);

#endif // RANDOM_KEYS_H_INCLUDED
