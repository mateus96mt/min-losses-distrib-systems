#ifndef INDIVIDUO_H_INCLUDED
#define INDIVIDUO_H_INCLUDED

#include <math.h>
#include "Graph_network.h"

struct Cromossomo{
    Arco *arco;
    double peso;
    int posicao;
};

class Individuo{
    private:
        int numArcos;
        double *pesos, perdaAtiva, perdaReativa;
    public:

        ///--------------------------conection to graph network--------------------------:

        static vector<Cromossomo*> cromossomos;
        static void criaCromossomos(Grafo *g);

        ///--------------------------conection to graph network--------------------------:


        Individuo(int numArcos);
        ~Individuo(){delete[] pesos;}

        /** GET's **/
        int getNumArcos(){return this->numArcos;}
        double *getPesos(){return this->pesos;}
        double getPerdaAtiva(){return this->perdaAtiva;}
        double getPerdaReativa(){return this->perdaReativa;}

        void geraPesosAleatorios();

        void geraPesosConfInicial(int *idsAbertos, int n, Grafo *g);

        void resetaPesos(float valor);

        void calculaFuncaoObjetivo(Grafo *g);

        void calculaFuncaoObjetivoOtimizado(Grafo *g);

        void imprimePesos();


//        ///--------------------------RKGA operators--------------------------:
//
//        void cruzamentoMedia(Individuo *pai, Individuo *filho);
//
//        void cruzamentoMedia2(Individuo *pai, Individuo *filho);
//
//        void mutacao();
//
//        ///--------------------------RKGA operators--------------------------:



//        ///----------------------Path-relingking operators-------------------:
//
//        Individuo *prs(Individuo *guia, Grafo *g, Individuo *indRef);//path relinking simples path de this para guia
//        Individuo *prs2(Individuo *guia, Grafo *g);//path relinking com modificacoes sugeridas luciana
//
//        ///----------------------Path-relingking operators-------------------:
};
#endif // INDIVIDUO_H_INCLUDED
