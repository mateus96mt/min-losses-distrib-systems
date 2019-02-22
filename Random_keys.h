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
        void geraPopAleatoriaConfInicial(Grafo *g, int *idsAbertos, int nAbertos);

        ~Random_keys(){}

        void ordenaPopulacaoAtual(Grafo *g);

        int avancaGeracoes(Grafo *g);
        int avancaGeracoes2(Grafo *g);
        int avancaGeracoesPRS(Grafo *g);//avanca geracoes aplicando pathrelinking simples(a cada geracao)

        void prsEvolutivo(vector<Individuo*> pool, vector<Individuo*> &populacao, Grafo *g);//path relinking simples evolutivo
        int avancaGeracoesPRSEvolutivoFinal(Grafo *g);


};
#endif // RANDOM_KEYS_H_INCLUDED
