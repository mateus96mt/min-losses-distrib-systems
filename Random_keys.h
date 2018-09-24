#ifndef RANDOM_KEYS_H_INCLUDED
#define RANDOM_KEYS_H_INCLUDED
#include <math.h>

#include "Individuo.h"

using namespace std;

class Random_keys{
    private:
        int numGeracoes, tamPop;

        vector<Individuo*> popAtual;
        vector<Individuo*> popAnterior;

    public:
        Random_keys(int tamPop, int numGeracoes);

        /** GET's **/
        vector<Individuo*> getPopAtual(){return this->popAtual;}
        vector<Individuo*> getPopAnterior(){return this->popAnterior;}
        int getNumGeracoes(){return this->numGeracoes;}
        int getTamPopulacao(){return this->tamPop;}


        void geraPopAleatoria(Grafo *g);
        void geraPopAleatoriaConfInicial(Grafo *g, int *idsAbertos, int n);

        ~Random_keys(){}

        void ordenaPopulacaoAtual(Grafo *g);

        void avancaGeracoes(Grafo *g);

};
#endif // RANDOM_KEYS_H_INCLUDED
